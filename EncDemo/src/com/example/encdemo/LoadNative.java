package com.example.encdemo;

public class LoadNative {
		
	static{
			System.loadLibrary("EncDemo");
	}
	public native int encrypt(String fin);
	public native int decrypt(String fin);
}
