package com.example.cooler;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;

public class MainActivity extends AppCompatActivity {
    private Handler handler = new Handler();
    private Runnable runnable = new Runnable() {
        @Override
        public void run() {
            downloadJSON("YourSite/get_service.php");
            handler.postDelayed(this, 1000);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        handler.postDelayed(runnable, 1000);
    }

    private void downloadJSON(String link) {

        class DownloadJSON extends AsyncTask<Void, Void, String> {

            @Override
            protected void onPreExecute() {
                super.onPreExecute();
            }


            @Override
            protected void onPostExecute(String s) {
                super.onPostExecute(s);
                try {
                    loadInto(s);
                } catch (JSONException e) {
                    Log.w("APP", e.getMessage() );
                    e.printStackTrace();
                }
            }

            @Override
            protected String doInBackground(Void... voids) {
                try {
                    URL url = new URL(link);
                    URLConnection conn = url.openConnection();
                    BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                    StringBuilder sb = new StringBuilder();
                    String line = null;

                    while((line = reader.readLine()) != null){
                        sb.append(line + "\n");
                        break;
                    }
                    return sb.toString().trim();
                } catch (Exception e) {
                    return new String("Exception: " + e.getMessage());
                }
            }
        }
        DownloadJSON getJSON = new DownloadJSON();
        getJSON.execute();
    }
    private void loadInto(String json) throws JSONException {
        JSONArray jsonArray = new JSONArray(json);
        String[] stocks = new String[jsonArray.length()];
        TextView[] coolers = new TextView[8];

        coolers[0] = (TextView)findViewById(R.id.WaterIndex1);
        coolers[1] = (TextView)findViewById(R.id.WaterIndex2);
        coolers[2] = (TextView)findViewById(R.id.WaterIndex3);
        coolers[3] = (TextView)findViewById(R.id.WaterIndex4);
        coolers[4] = (TextView)findViewById(R.id.WaterIndex5);
        coolers[5] = (TextView)findViewById(R.id.WaterIndex6);
        coolers[6] = (TextView)findViewById(R.id.WaterIndex7);
        coolers[7] = (TextView)findViewById(R.id.WaterIndex8);

        for (int i = 0; i < jsonArray.length(); i++) {
            JSONObject obj = jsonArray.getJSONObject(i);
            coolers[i].setText("Вода: " + obj.getString("Value") + "%");
        }
    }
}