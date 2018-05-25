package org.robin.activity;

import org.robin.camera.CameraInterface;
import org.robin.camera.CameraInterface.CamOpenOverCallback;
import org.robin.camera.preview.CameraSurfaceView;
import org.robin.util.DisplayUtil;
import org.robin.playcamera.R;

import android.app.Activity;
import android.graphics.Point;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.ImageButton;
import android.widget.TextView;

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
public class CameraActivity extends Activity implements CamOpenOverCallback,CameraSurfaceView.OnSurfaceHolderLister {
	private static final String TAG = "TFrobin_activity";
	CameraSurfaceView surfaceView = null;
	ImageButton shutterBtn;
	float previewRate = -1f;
	private Executor singleThreadExecutor = Executors.newSingleThreadExecutor();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Thread openThread = new Thread(){
			@Override
			public void run() {
				// TODO Auto-generated method stub
				CameraInterface.getInstance().doOpenCamera(CameraActivity.this);
			}
		};
		openThread.start();
		setContentView(R.layout.activity_camera);
		initUI();
		initViewParams();
		
		shutterBtn.setOnClickListener(new BtnListeners());
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.camera, menu);
		return true;
	}

	private void initUI(){
		surfaceView = (CameraSurfaceView)findViewById(R.id.camera_surfaceview);
        surfaceView.setHolderCallback(this);
		shutterBtn = (ImageButton)findViewById(R.id.btn_shutter);
	}
	private void initViewParams(){
		LayoutParams params = surfaceView.getLayoutParams();
		Point p = DisplayUtil.getScreenMetrics(this);
		params.width = p.x;
		params.height = p.y;
		previewRate = DisplayUtil.getScreenRate(this); //Ĭ��ȫ���ı���Ԥ��
		surfaceView.setLayoutParams(params);

		//�ֶ���������ImageButton�Ĵ�СΪ120dip��120dip,ԭͼƬ��С��64��64
		LayoutParams p2 = shutterBtn.getLayoutParams();
		p2.width = DisplayUtil.dip2px(this, 80);
		p2.height = DisplayUtil.dip2px(this, 80);;		
		shutterBtn.setLayoutParams(p2);	

	}

	@Override
	public void cameraHasOpened() {
		// TODO Auto-generated method stub
		SurfaceHolder holder = surfaceView.getSurfaceHolder();
		CameraInterface.getInstance().doStartPreview(holder, previewRate);
	}
	
	private class BtnListeners implements OnClickListener{

		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			switch(v.getId()){
			case R.id.btn_shutter:
				CameraInterface.getInstance().doTakePicture();
				break;
			default:break;
			}
		}

	}

    @Override
    public void surfaceHolderCreate(SurfaceHolder holder){
		while(CameraInterface.getInstance().mCamera == null);
        CameraInterface.getInstance().SetSurfaceHolder(holder);
    }
	@Override
	protected void onDestroy() {
		super.onDestroy();
//		singleThreadExecutor.execute(new Runnable() {
//			@Override
//			public void run() {
//				//do something later
//				Log.i(TAG, "onDestroy");
//			}
//		});
	}
}
