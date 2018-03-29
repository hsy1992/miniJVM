
import org.luaj.vm2.*;
import org.luaj.vm2.lib.jme.JmePlatform;

public class Sample {

    public static void main(String[] args) {
        //System.setProperty("luaj.package.path","./examples/lua/");
        Sample sam = new Sample();
        sam.startApp();
    }

    // the script will be loaded as a resource 
    private static final String DEFAULT_SCRIPT = "./hello.lua";

    protected void startApp() {
        // get the script as an app property
        String script = null;
        if (script == null) {
            script = DEFAULT_SCRIPT;
        }

        // create an environment to run in
        Globals globals = JmePlatform.standardGlobals();
        LuaValue chunk = globals.loadfile(script);
//        globals.get("require").call(LuaValue.valueOf(script));
        chunk.call( LuaValue.valueOf(script) );
    }

    protected void destroyApp(boolean arg0) {
    }

    protected void pauseApp() {
    }

}
