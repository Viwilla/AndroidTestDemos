package com.example.aesbase64enc;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;


public class MainActivity extends Activity {

	
	public native String encData(String str);
	public native String decData(String str);
	static{
		System.loadLibrary("AESEnc");
	}
	Button EncButton;
	Button DecButton;
	EditText tv1;
	TextView encstrText;
	TextView decstrText;
	String str;
	String str1 ;
	String str2;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        EncButton = (Button)findViewById(R.id.encButton);
		DecButton = (Button)findViewById(R.id.decButton);
		tv1=(EditText)findViewById(R.id.editText);
		encstrText = (TextView)findViewById(R.id.encstr);
		decstrText = (TextView)findViewById(R.id.decstr);
		/**
        String str = "qwertyuisfdlsajdxcvnkhsakfh1332487";
        String str1 = encData(str);
        Log.i("after encrypt", str1);
        Log.i("after decrypt", decData(str1));**/
    
	    EncButton.setOnClickListener(new OnClickListener(){
			public void onClick(View v){
				str=tv1.getText().toString();
				str1 = null;
				if (str == ""){
					String msg = "请选择文件！";
					showDialog(msg);
					}  
				else{
					str1 = encData(str);
					long startTime=System.currentTimeMillis(); 
					if(str1!=null){
						long endTime=System.currentTimeMillis();
						String msg = "加密成功，用时" + (endTime-startTime) + "ms (" + (endTime-startTime)/1000 + "s) \n";
						showDialog(msg);
						encstrText.setText(str1);
					}
					else{
						showDialog("加密失败");
					}
				}
			}

		});
		
		DecButton.setOnClickListener(new OnClickListener(){
			public void onClick(View v){
				if (str1 == null){
					String msg = "请先加密！";
					showDialog(msg);
				}  
				else{
					long startTime=System.currentTimeMillis(); 
					str2 = decData(str1);
					if((str2!=null)){
						long endTime=System.currentTimeMillis();
						String msg = "解密成功，用时" + (endTime-startTime) + "ms (" + (endTime-startTime)/1000 + "s) \n";
						showDialog(msg);
						decstrText.setText(str2);
					}
					else{
						showDialog("解密失败！");
					}
				}
			}
		});
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
    
	private void showDialog(String str){

		new AlertDialog.Builder(MainActivity.this).setTitle("系统提示")//设置对话框
		.setMessage(str) //设置显示的内容  
		.setPositiveButton("确定",new DialogInterface.OnClickListener() {//添加确定按钮  
			@Override  
			public void onClick(DialogInterface dialog, int which) {//确定按钮的响应事件  
				// TODO Auto-generated method stub
				
				}
			}).setNegativeButton("返回",new DialogInterface.OnClickListener() {//添加返回按钮  
				@Override  
				public void onClick(DialogInterface dialog, int which) {//响应事件  
					// TODO Auto-generated method stub  
					//Log.i("alertdialog"," 请选择文件！");  
					}  
				}).show();	
	}
}
