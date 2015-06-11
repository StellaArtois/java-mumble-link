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

	public static void main(String[] args) {
		try {
			MumbleLink.loadLibrary();
			MumbleLink mumbleLink = new MumbleLink("1", "1");
			mumbleLink.setIdentityAndContext("User", "Server");
			mumbleLink.updateMumble(
						1f, 0f, 1f,
						0f, 0f, 0f,
						1f, 1f, 0f);
			mumbleLink.deinit();
		} catch (Exception e) {
			System.err.println("Error in [MumbleLink library]: " + e.toString());
		}
	}
}
