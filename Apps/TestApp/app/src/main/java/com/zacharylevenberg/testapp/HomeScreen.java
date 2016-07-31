package com.zacharylevenberg.testapp;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Set;


public class HomeScreen extends AppCompatActivity {

    // Bluetooth variables.
    private static final int REQUEST_ENABLE_BT = 1234;
    private BluetoothAdapter mBluetoothAdapter;
    private Set<BluetoothDevice> pairedDevices;
    ListView lv;

    //timer variables
    EditText timerText;
    private long startTime = 0;
    private int timerState = 0;

    //runs without a timer by reposting this handler at the end of the runnable
    Handler timerHandler = new Handler();
    Runnable timerRunnable = new Runnable() {

        @Override
        public void run() {
            long millis = System.currentTimeMillis() - startTime;
            int seconds = (int) (millis / 1000);
            int minutes = seconds / 60;
            seconds = seconds % 60;

            timerText.setText(String.format("%d:%02d", minutes, seconds));

            timerHandler.postDelayed(this, 500);
        }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //display this activities layout
        setContentView(R.layout.activity_home_screen);

        //Get the intent that was passed
        //Get the string using the key
        Intent intent = getIntent();
        String name = intent.getStringExtra(com.zacharylevenberg.testapp.WelcomeScreen.EXTRA_MESSAGE);
        StringBuilder sb = new StringBuilder(100);
        sb.append("Wassup, ");
        sb.append(name);
        sb.append(".\nWelcome to your E-Moto\n");

        //Set the text to the textview resource
        TextView textView = (TextView)(findViewById(R.id.message_text));
        textView.setText(sb);
        textView.setTextColor(ContextCompat.getColor(this, R.color.colorPrimary));

        // Ensures Bluetooth is available on the device and it is enabled. If not,
        // displays a dialog requesting user permission to enable Bluetooth.
        final BluetoothManager bluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = bluetoothManager.getAdapter();
        if (mBluetoothAdapter == null || !mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            Intent getVisibleIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
            startActivityForResult(getVisibleIntent, 0);
        }

        //Set the list view
        lv = (ListView)findViewById(R.id.listView);
        listit();

        //set the timer display
        timerText = (EditText) findViewById(R.id.timer);
    }

    //When we arrive at this state again some time
    @Override
    public void onResume() {
        super.onResume();
        //Do stuff here
    }

    //When we leave this state...
    @Override
    public void onPause() {
        super.onPause();
        //Do stuff here
        timerHandler.removeCallbacks(timerRunnable);
    }

    public void startTimer(View v) {
        if (timerState == 1) {
            timerState = 0;
            timerHandler.removeCallbacks(timerRunnable);
        } else {
            timerState = 1;
            startTime = System.currentTimeMillis();
            timerHandler.postDelayed(timerRunnable, 0);
        }
    }

    public void listit(){
        pairedDevices = mBluetoothAdapter.getBondedDevices();
        ArrayList<String> list=new ArrayList<String>();
        for(BluetoothDevice bt : pairedDevices)
            list.add(bt.getName());
        final ArrayAdapter adapter = new ArrayAdapter(getApplicationContext(),android.R.layout.simple_list_item_1, list);
        lv.setAdapter(adapter);
    }


}
