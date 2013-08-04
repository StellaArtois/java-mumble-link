package net.aib42.mumblelink;

public class MumbleLink
{
	public static void loadLibrary() {
		boolean is64bit = System.getProperty("sun.arch.data.model").equalsIgnoreCase("64");

		//Launcher takes care of extracting natives
		if( is64bit )
			System.loadLibrary( "JMumbleLibrary64" );
		else
			System.loadLibrary( "JMumbleLibrary" );
	}

	public MumbleLink(String name, String description)
	{
		setNameAndDescription(name, description);
		init();
	}
	public native void init();
	public native void deinit();

	private native void setNameAndDescription(String name, String description);
	public native void setIdentityAndContext(String identity, String context);
	public native void updateMumble(
		float avatarPosX, float avatarPosY, float avatarPosZ,
		float avatarForwardX, float avatarForwardY, float avatarForwardZ,
		float avatarUpX, float avatarUpY, float avatarUpZ
	);
}
