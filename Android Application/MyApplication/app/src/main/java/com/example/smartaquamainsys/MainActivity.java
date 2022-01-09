package com.example.smartaquamainsys;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TextView;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {
    private EditText ipAddInBox;
    private static Button connect;
    private TextView instOne;
    private TextView Readtest;
    public static String ipAddress;
    private int connMethod = 0;
    private String connCheck;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        connect=(Button) findViewById(R.id.connect);
        ipAddInBox=(EditText) findViewById(R.id.ipaddr);
        Switch s1 = findViewById(R.id.switch1);       // Switch button
        instOne=(TextView) findViewById(R.id.Inst1);
        Readtest=(TextView) findViewById(R.id.textView);

        s1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(b){
                    //internet connection method
                    connMethod = 1;
                }
                else{
                    // direct connection method
                    connMethod = 0;
                }

            }
        });




        connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ipAddress=ipAddInBox.getText().toString();
                //new getD().execute();
                Readtest.setText("CONNS");

                connCheck = (String) Readtest.getText();

                if (connCheck == "CONNS"){

                   // ManLightOn process1 = new ManLightOn();
                    //process1.execute();
                    Intent ht1 = new Intent(MainActivity.this,LoginPage.class);
                    startActivity(ht1);
                }
                else{
                         // Toast Error display
                }

            }
        });


    }

    //Inner class
    public class getD extends AsyncTask<Void,Void,Void>{
        String words;

        @Override
        protected Void doInBackground(Void... voids) {
            try {
                Document doc = Jsoup.connect("http://" + ipAddress +" /CN").get();
                words = doc.text();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void unused) {
            super.onPostExecute(unused);
            Readtest.setText(words);
        }
    }


}
