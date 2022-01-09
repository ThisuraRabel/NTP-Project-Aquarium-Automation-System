package com.example.smartaquamainsys;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

public class ManControl extends AppCompatActivity {

    Button LOn, ManConn, LOff, PHLow, PHHigh;

    Switch FNow;
    SeekBar FSeek;
    TextView amView;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_man_control);

        LOn = findViewById(R.id.LampOnBtn);
        LOff = findViewById(R.id.LampOffBtn);
        PHLow = findViewById(R.id.PHLowBtn);
        PHHigh = findViewById(R.id.PHHighBtn);
        FNow = findViewById(R.id.FeedNSwitch);
        FSeek = findViewById(R.id.FoodSeek);
        ManConn = findViewById(R.id.FeedBtn);
        amView = findViewById(R.id.textView34);

        amView.setVisibility(View.INVISIBLE);
        FSeek.setVisibility(View.INVISIBLE);

        FNow.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(b){
                    amView.setVisibility(View.VISIBLE);
                    FSeek.setVisibility(View.VISIBLE);
                }
                else{
                    amView.setVisibility(View.INVISIBLE);
                    FSeek.setVisibility(View.INVISIBLE);
                }

            }
        });


    }


}