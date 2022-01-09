package com.example.smartaquamainsys;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TextView;

public class ChangeDetails extends AppCompatActivity {


    EditText cName, cPNumber, cCPass, cNewPass, cReNewPass;
    Switch cSwitchCPass;
    Button changeDetSub;
    TextView cCp, cNp, cRep;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_change_details);

        cName= findViewById(R.id.textchangeName);
        cPNumber= findViewById(R.id.textPnumChange);
        cCPass= findViewById(R.id.textVCurrentPass);
        cNewPass= findViewById(R.id.textNewPass);
        cReNewPass= findViewById(R.id.textRepassChange);
        cSwitchCPass= findViewById(R.id.ChangepassSwitch);
        changeDetSub= findViewById(R.id.changeDetailsBtn);
        cCp= findViewById(R.id.textcp);
        cNp= findViewById(R.id.textnp);
        cRep= findViewById(R.id.textrep);


        cCPass.setVisibility(View.INVISIBLE);
        cNewPass.setVisibility(View.INVISIBLE);
        cReNewPass.setVisibility(View.INVISIBLE);
        cCp.setVisibility(View.INVISIBLE);
        cNp.setVisibility(View.INVISIBLE);
        cRep.setVisibility(View.INVISIBLE);

        cSwitchCPass.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(b){
                    cCPass.setVisibility(View.VISIBLE);
                    cNewPass.setVisibility(View.VISIBLE);
                    cReNewPass.setVisibility(View.VISIBLE);
                    cCp.setVisibility(View.VISIBLE);
                    cNp.setVisibility(View.VISIBLE);
                    cRep.setVisibility(View.VISIBLE);
                }
                else{
                    cCPass.setVisibility(View.INVISIBLE);
                    cNewPass.setVisibility(View.INVISIBLE);
                    cReNewPass.setVisibility(View.INVISIBLE);
                    cCp.setVisibility(View.INVISIBLE);
                    cNp.setVisibility(View.INVISIBLE);
                    cRep.setVisibility(View.INVISIBLE);
                }

            }
        });


        /*
         amView.setVisibility(View.VISIBLE);
                    FSeek.setVisibility(View.VISIBLE);
         */


    }
}