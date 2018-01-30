package com.example.encdemo;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.lang.System;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.SimpleAdapter;
import android.widget.TextView;


public class MainActivity extends Activity {

	private Button chooseFile;
	private TextView fileDire;
	String 	fileslected = ""; 
	private String curPath = new String("/storage/sdcard0/");
	Button EncButton;
	Button DecButton;
	private LoadNative s;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		chooseFile = (Button)findViewById(R.id.selectButton);
		fileDire = (TextView)findViewById(R.id.textChooseFile);
		EncButton = (Button)findViewById(R.id.Encbutton);
		DecButton = (Button)findViewById(R.id.Decbutton);
		chooseFile.setOnClickListener(new OnClickListener() {

			public void onClick(View v) {
			openCurDir(curPath);	
			}
		});
		
		EncButton.setOnClickListener(new OnClickListener(){
			public void onClick(View v){
				if (fileslected == ""){
					String msg = "��ѡ���ļ���";
					showDialog(msg);
					}  
				else{
					String outfile = fileslected + ".enc"; 
					s = new LoadNative();
					long startTime=System.currentTimeMillis(); 
					if((s.encrypt(fileslected))==0){
						long endTime=System.currentTimeMillis();
						String msg = "���ܳɹ�����ʱ" + (endTime-startTime) + "ms (" + (endTime-startTime)/1000 + "s) \n-->" + outfile;
						showDialog(msg);
					}
					else{
						showDialog("����ʧ��");
					}
				}
			}
		});
		
		DecButton.setOnClickListener(new OnClickListener(){
			public void onClick(View v){
				if (fileslected == ""){
					String msg = "��ѡ���ļ���";
					showDialog(msg);
				}  
				else{
					String outfile = fileslected + ".dec";
					long startTime=System.currentTimeMillis(); 				 
					s = new LoadNative();
					if((s.decrypt(fileslected))==0){
						long endTime=System.currentTimeMillis();
						String msg = "���ܳɹ�����ʱ" + (endTime-startTime) + "ms (" + (endTime-startTime)/1000 + "s) \n-->" + outfile;
						showDialog(msg);
					}
					else{
						showDialog("����ʧ�ܣ�");
					}
				}
			}
		});
	}
	
	
	private void showDialog(String str){

		new AlertDialog.Builder(MainActivity.this).setTitle("ϵͳ��ʾ")//���öԻ���
		.setMessage(str) //������ʾ������  
		.setPositiveButton("ȷ��",new DialogInterface.OnClickListener() {//���ȷ����ť  
			@Override  
			public void onClick(DialogInterface dialog, int which) {//ȷ����ť����Ӧ�¼�  
				// TODO Auto-generated method stub
				
				}
			}).setNegativeButton("����",new DialogInterface.OnClickListener() {//��ӷ��ذ�ť  
				@Override  
				public void onClick(DialogInterface dialog, int which) {//��Ӧ�¼�  
					// TODO Auto-generated method stub  
					Log.i("alertdialog"," ��ѡ���ļ���");  
					}  
				}).show();	
	}
	
	private String openCurDir(String curPath){
		
	    File f = new File(curPath);
	    File[] file = f.listFiles();
	    final List<Map<String,Object>> listItem = new ArrayList<Map<String,Object>>();

	    if(!curPath.equals("/")){//������Ǹ�Ŀ¼�Ļ�����Ҫ��ʾ���б��м������
	        Map<String,Object> map1=new HashMap<String,Object>();
	        map1.put("name", "������һ��Ŀ¼");
	        map1.put("image", R.drawable.back02);
	        map1.put("path",f.getParent());
	        map1.put("isDire", true);
	        listItem.add(map1);
	    }
	    
	    if(file != null){//�����ж� ����Ŀ¼Ϊ�յ�ʱ��ᱨ��
	        for(int i = 0; i < file.length; i++){
	            Map<String,Object> map=new HashMap<String,Object>();
	            map.put("name", file[i].getName());
	            map.put("image", (file[i].isDirectory()) ? R.drawable.folder : R.drawable.doc);
	            map.put("path",file[i].getPath());
	            map.put("isDire", file[i].isDirectory());
	            listItem.add(map);
	        }
	    }
	
	    SimpleAdapter adapter = new SimpleAdapter(MainActivity.this,listItem,R.layout.adapter,
                new String[]{"name","image"},new int[]{R.id.adapter_filename,R.id.adapter_image});
        
        final AlertDialog.Builder b =new Builder(MainActivity.this);
        b.setAdapter(adapter, new DialogInterface.OnClickListener() {
            
            @Override
            public void onClick(DialogInterface arg0, int arg1) {
                // TODO Auto-generated method stub
                if((Boolean) listItem.get(arg1).get("isDire")){
                    openCurDir((String)listItem.get(arg1).get("path"));
                }else{
                	fileslected = (String)listItem.get(arg1).get("path");
                    fileDire.setText(fileslected);
                }
            }
        });
        b.show();
        
        return fileslected;
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
}
