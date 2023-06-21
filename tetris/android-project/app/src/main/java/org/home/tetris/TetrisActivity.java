package org.home.tetris;

import org.libsdl.app.SDLActivity;

public class TetrisActivity extends SDLActivity {
    @Override
    protected String getMainFunction() {
        return "main";
    }

    @Override
    protected String[] getLibraries() {
        return new String[] {
                "c++_shared",
                "SDL3",
                "SDL3_image",
                // "SDL3_mixer",
                // "SDL3_net",
                // "SDL3_ttf",
                "nano",
                "tetris",
        };
    }
}

//class TetrisMain implements Runnable {
//    @Override
//    public void run() {
//        String library = TetrisActivity.mSingleton.getMainSharedObject();
//        String function = TetrisActivity.mSingleton.getMainFunction();
//        String[] arguments = TetrisActivity.mSingleton.getArguments();
//
//        try {
//            android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_DISPLAY);
//        } catch (Exception e) {
//            Log.v("TETRIS", "modify thread properties failed " + e.toString());
//        }
//
//        Log.v("TETRIS", "Running main function " + function + " from library " + library);
//
//        TetrisActivity.nativeRunMain(library, function, arguments);
//
//        Log.v("TETRIS", "Finished main function");
//
//        if (TetrisActivity.mSingleton != null && !TetrisActivity.mSingleton.isFinishing()) {
//            // Let's finish the Activity
//            TetrisActivity.mSDLThread = null;
//            TetrisActivity.mSDLMainFinished = true;
//            TetrisActivity.mSingleton.finish();
//        }  // else: Activity is already being destroyed
//
//    }
//}
