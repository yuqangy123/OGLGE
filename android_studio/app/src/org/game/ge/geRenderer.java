package org.game.ge;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by Administrator on 2018/3/3.
 */

public class geRenderer implements GLSurfaceView.Renderer {

    private final static long NANOSECONDSPERSECOND = 1000000000L;
    private final static long NANOSECONDSPERMICROSECOND = 1000000;
    private long mLastTickInNanoSeconds;
    private int mScreenWidth;
    private int mScreenHeight;

    private static long sAnimationInterval = (long) (1.0 / 60 * NANOSECONDSPERSECOND);

    @Override
    public void onSurfaceCreated(final GL10 GL10, final EGLConfig EGLConfig) {
        geRenderer.nativeInit(mScreenWidth, mScreenHeight);
        this.mLastTickInNanoSeconds = System.nanoTime();
    }


    @Override
    public void onSurfaceChanged(final GL10 GL10, final int width, final int height) {
        geRenderer.nativeOnSurfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(final GL10 gl) {
        /*
         * No need to use algorithm in default(60 FPS) situation,
         * since onDrawFrame() was called by system 60 times per second by default.
         */
        if (sAnimationInterval <= 1.0 / 60 * NANOSECONDSPERSECOND) {
            geRenderer.nativeRender();
        } else {
            final long now = System.nanoTime();
            final long interval = now - this.mLastTickInNanoSeconds;

            if (interval < sAnimationInterval) {
                try {
                    Thread.sleep((sAnimationInterval - interval) / NANOSECONDSPERMICROSECOND);
                } catch (final Exception e) {
                }
            }
            /*
             * Render time MUST be counted in, or the FPS will slower than appointed.
            */
            this.mLastTickInNanoSeconds = System.nanoTime();
            geRenderer.nativeRender();
        }
    }

    public void setScreenWidthAndHeight(final int surfaceWidth, final int surfaceHeight) {
        mScreenWidth = surfaceWidth;
        mScreenHeight = surfaceHeight;
    }

    private static native void nativeRender();
    private static native void nativeInit(final int width, final int height);
    private static native void nativeOnSurfaceChanged(final int width, final int height);
}
