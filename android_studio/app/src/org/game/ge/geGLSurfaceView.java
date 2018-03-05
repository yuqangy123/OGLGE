package org.game.ge;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.util.Log;
import android.view.inputmethod.InputMethodManager;

/**
 * Created by Administrator on 2018/3/3.
 */

public class geGLSurfaceView extends GLSurfaceView {
    static geGLSurfaceView mgeGLSurfaceView;
    static geRenderer mgeRender;
    public geGLSurfaceView(final Context context) {
        super(context);

        this.initView();
    }

    public geGLSurfaceView(final Context context, final AttributeSet attrs) {
        super(context, attrs);

        this.initView();
    }

    protected void initView() {
        this.setEGLContextClientVersion(2);
        this.setFocusableInTouchMode(true);

        mgeGLSurfaceView = this;
    }

    // ===========================================================
    // Getter & Setter
    // ===========================================================


    public static geGLSurfaceView getInstance() {
        return mgeGLSurfaceView;
    }

    public void setGeRenderer(final geRenderer renderer) {
        mgeRender = renderer;
        this.setRenderer(mgeRender);
    }


    /*
     * This function is called before Cocos2dxRenderer.nativeInit(), so the
     * width and height is correct.
     */
    @Override
    protected void onSizeChanged(final int pNewSurfaceWidth, final int pNewSurfaceHeight, final int pOldSurfaceWidth, final int pOldSurfaceHeight) {
        if(!this.isInEditMode()) {
            this.mgeRender.setScreenWidthAndHeight(pNewSurfaceWidth, pNewSurfaceHeight);
        }
    }
}
