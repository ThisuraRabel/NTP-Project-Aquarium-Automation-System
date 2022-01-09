package com.example.smartaquamainsys;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;

public class ChangeDeviceSettings extends AppCompatActivity {

     EditText PHMin, PHMax, LI, TMax, TMin, FTimes;
     SeekBar FeedAmt;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_change_device_settings);

        PHMin= findViewById(R.id.TextPHMin);
        PHMax= findViewById(R.id.TextPHMax);
        LI= findViewById(R.id.textLightInt);
        TMax=  findViewById(R.id.textTMin);
        TMin=  findViewById(R.id.textTmax);
        FTimes=  findViewById(R.id.TextFamt);
        FeedAmt=  findViewById(R.id.FeedamtSeek);

    }
}