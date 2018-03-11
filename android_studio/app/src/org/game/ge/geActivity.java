package org.game.ge;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.ViewGroup;

/**
 * Created by Administrator on 2018/3/3.
 */

public class geActivity extends Activity {

    protected geLayout mFrameLayout;
    protected geGLSurfaceView mGLSurfaceView;
    private int[] mGLContextAttrs;

    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        onLoadNativeLibraries();
        init();
    }

    protected void  onLoadNativeLibraries() {
        try {
            ApplicationInfo ai = getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
            Bundle bundle = ai.metaData;
            String libName = bundle.getString("android.app.lib_name");
            System.loadLibrary(libName);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    protected void init(){
        // FrameLayout
        ViewGroup.LayoutParams framelayout_params =
                new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                        ViewGroup.LayoutParams.MATCH_PARENT);

        mFrameLayout = new geLayout(this);
        mFrameLayout.setLayoutParams(framelayout_params);

        mGLSurfaceView = onCreateView();
        mFrameLayout.addView(mGLSurfaceView);
        mGLSurfaceView.setGeRenderer(new geRenderer());

        setContentView(mFrameLayout);
    }

    public geGLSurfaceView onCreateView() {
        geGLSurfaceView glSurfaceView = new geGLSurfaceView(this);

        //_glContextAttrs.redBits, _glContextAttrs.greenBits, _glContextAttrs.blueBits,_glContextAttrs.alphaBits, _glContextAttrs.depthBits, _glContextAttrs.stencilBits
        mGLContextAttrs = new int[6];
        mGLContextAttrs[0] = 5;
        mGLContextAttrs[1] = 6;
        mGLContextAttrs[2] = 5;
        mGLContextAttrs[3] = 0;
        mGLContextAttrs[4] = 16;
        mGLContextAttrs[5] = 0;

        //this line is need on some device if we specify an alpha bits
        if(this.mGLContextAttrs[3] > 0) glSurfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);

       // Cocos2dxEGLConfigChooser chooser = new Cocos2dxEGLConfigChooser(this.mGLContextAttrs);
        //glSurfaceView.setEGLConfigChooser(chooser);

        return glSurfaceView;
    }

    @Override
    protected void onPause() {
        super.onPause();
            mGLSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLSurfaceView.onResume();
    }
}
