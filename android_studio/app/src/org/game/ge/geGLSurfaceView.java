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


        //Render the view only when there is a change in the drawing
        //setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

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
        /*
        设置渲染模式。 当renderMode为RENDERMODE_CONTINUOUSLY时，会重复调用渲染器以重新渲染场景。 当renderMode为RENDERMODE_WHEN_DIRTY时，仅在创建曲面时或在调用requestRender时才渲染渲染器。 默认为RENDERMODE_CONTINUOUSLY。

        使用RENDERMODE_WHEN_DIRTY可以通过允许GPU和CPU在视图不需要更新时空闲，从而延长电池寿命和整体系统性能。

        此方法只能在setRenderer（Renderer）之后调用
        */
        this.setRenderMode(RENDERMODE_WHEN_DIRTY);
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
