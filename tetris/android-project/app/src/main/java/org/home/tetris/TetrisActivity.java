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