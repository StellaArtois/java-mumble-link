#include <string>
#ifdef WIN32
#include <windows.h>
#else

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>          /* For getuid */
#include <sys/types.h>
#include <cstdlib>           /* For mbstowcs */
#define nullptr NULL

#endif

#include <jni.h>

#include "net_aib42_mumblelink_MumbleLink.h"
#include <stdint.h>

struct LinkedMem {
	uint32_t uiVersion;
	uint32_t uiTick;
	float	fAvatarPosition[3];
	float	fAvatarFront[3];
	float	fAvatarTop[3];
	wchar_t	name[256];
	float	fCameraPosition[3];
	float	fCameraFront[3];
	float	fCameraTop[3];
	wchar_t	identity[256];
	uint32_t context_len;
	char context[256];
	wchar_t description[2048];
};


static LinkedMem* lm = nullptr;

static struct {
	std::string name;
	std::string description;
	std::string identity;
	std::string context;
	bool update;
} linkInfo;

void writeString(wchar_t* dst, std::string& src, size_t dstSize)
{
#ifdef WIN32
	MultiByteToWideChar(CP_UTF8, 0, src.c_str(), src.length(), dst, dstSize);
#else
	mbstowcs( dst, src.c_str(), dstSize );
#endif
}

#ifdef WIN32
static HANDLE hMap;
#else
int shmfd;
#endif

JNIEXPORT void JNICALL Java_net_aib42_mumblelink_MumbleLink_init (JNIEnv *, jobject)
{
	linkInfo.update = true;
#ifdef WIN32
	hMap = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
	if (hMap != NULL) {
		lm = reinterpret_cast<LinkedMem*>(MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0));
	}
#else
	char memname[256];
	snprintf(memname, 256, "/MumbleLink.%d", getuid());

	shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

	if (shmfd < 0) {
		return;
	}

	lm = (LinkedMem *)(mmap(NULL, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0));

	if (lm == (void *)(-1)) {
		lm = NULL;
		return;
	}

#endif
}

JNIEXPORT void JNICALL Java_net_aib42_mumblelink_MumbleLink_deinit(JNIEnv *, jobject)
{
#ifdef WIN32
	if (hMap != NULL) {
		if (lm != nullptr) {
			UnmapViewOfFile(hMap);
		}
		CloseHandle(hMap);
	}
#else
	if( lm ) munmap(lm, sizeof(struct LinkedMem));
	if( shmfd >-1 ) close( shmfd );

#endif
}


void readJNIString(JNIEnv* env, std::string& dst, jstring src)
{
	const char* chars = env->GetStringUTFChars(src, nullptr);
	if (chars != nullptr) {
		size_t charCount = env->GetStringUTFLength(src);
		dst.assign(chars, charCount);
		env->ReleaseStringUTFChars(src, chars);
	}
}

JNIEXPORT void JNICALL Java_net_aib42_mumblelink_MumbleLink_setNameAndDescription(
	JNIEnv* env, jobject,
	jstring name, jstring description
) {
	readJNIString(env, linkInfo.name, name);
	readJNIString(env, linkInfo.description, description);
}

JNIEXPORT void JNICALL Java_net_aib42_mumblelink_MumbleLink_setIdentityAndContext(
	JNIEnv* env, jobject,
	jstring identity, jstring context
) {
	readJNIString(env, linkInfo.identity, identity);
	readJNIString(env, linkInfo.context, context);
	linkInfo.update = true;
}

JNIEXPORT void JNICALL Java_net_aib42_mumblelink_MumbleLink_updateMumble(
	JNIEnv*, jobject,
	jfloat avatarPosX, jfloat avatarPosY, jfloat avatarPosZ,
	jfloat avatarForwardX, jfloat avatarForwardY, jfloat avatarForwardZ,
	jfloat avatarUpX, jfloat avatarUpY, jfloat avatarUpZ
) {
	if (lm == nullptr) return;

	if (lm->uiVersion != 2) {
		writeString(lm->name, linkInfo.name, sizeof(lm->name) / sizeof(*lm->name));
		writeString(lm->description, linkInfo.description, sizeof(lm->description) / sizeof(*lm->description));
		lm->uiVersion = 2;
	}
	lm->uiTick++;

	lm->fAvatarPosition[0] = avatarPosX;
	lm->fAvatarPosition[1] = avatarPosY;
	lm->fAvatarPosition[2] = avatarPosZ;

	lm->fAvatarFront[0] = avatarForwardX;
	lm->fAvatarFront[1] = avatarForwardY;
	lm->fAvatarFront[2] = avatarForwardZ;

	lm->fAvatarTop[0] = avatarUpX;
	lm->fAvatarTop[1] = avatarUpY;
	lm->fAvatarTop[2] = avatarUpZ;

	lm->fCameraPosition[0] = avatarPosX;
	lm->fCameraPosition[1] = avatarPosY;
	lm->fCameraPosition[2] = avatarPosZ;

	lm->fCameraFront[0] = avatarForwardX;
	lm->fCameraFront[1] = avatarForwardY;
	lm->fCameraFront[2] = avatarForwardZ;

	lm->fCameraTop[0] = avatarUpX;
	lm->fCameraTop[1] = avatarUpY;
	lm->fCameraTop[2] = avatarUpZ;

	if( linkInfo.update ) {
		writeString(lm->identity, linkInfo.identity, sizeof(lm->identity) / sizeof(*lm->identity));
		linkInfo.context.copy(lm->context, sizeof(lm->context), 0);
		lm->context_len = linkInfo.context.length();
		linkInfo.update = false;
	}
}
