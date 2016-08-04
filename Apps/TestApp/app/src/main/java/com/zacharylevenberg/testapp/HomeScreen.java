package com.zacharylevenberg.testapp;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.WindowManager;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Set;


public class HomeScreen extends AppCompatActivity {

    // Bluetooth variables.
    private static final int REQUEST_ENABLE_BT = 1234;
    private static final int SCANNING_TIME = 10000;
    private BluetoothAdapter mBluetoothAdapter;
    private Set<BluetoothDevice> pairedDevices;

    //Paried Device List
    ListView bondedDevicesView;
    ArrayList<String> pairedDeviceList=new ArrayList<String>();
    ArrayAdapter bondedDeviceAdapter;

    //Discoverable Device List
    public TextView availDeviceText;
    public ListView availDevicesView;
    public ArrayList<String> availDeviceList=new ArrayList<String>();
    public ArrayAdapter availDeviceAdapter;

    //timer variables
    public EditText timerText;
    private long startTime = 0;
    private int timerState = 0;
    private static long millis;




    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //display this activities layout
        setContentView(R.layout.activity_home_screen);
        //Suppress the keypad in this activity
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);

        //Get the intent that was passed to this activity
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
            //Intent getVisibleIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
            //startActivityForResult(getVisibleIntent, 0);
        }

        //Start scan and start timer
        scanForBluetooth();

        //initialize the list view
        availDevicesView = (ListView)findViewById(R.id.availableDevicesList);
        availDeviceAdapter = new ArrayAdapter(getApplicationContext(),android.R.layout.simple_list_item_1, availDeviceList);
        availDevicesView.setAdapter(availDeviceAdapter);

/*        //Set the list view
        bondedDevicesView = (ListView)findViewById(R.id.listView);
        bondedDeviceAdapter = new ArrayAdapter(getApplicationContext(),android.R.layout.simple_list_item_1, pairedDeviceList);
        bondedDevicesView.setAdapter(bondedDeviceAdapter);
        listit();*/

        //set the timer display
        timerText = (EditText) findViewById(R.id.timer);
        availDeviceText = (TextView)(findViewById(R.id.availableDevices));
    }



    //When we arrive at this state again some time
    @Override
    public void onResume() {
        super.onResume();
        if(timerState == 1) {
            secondsTimer.postDelayed(secondsTimerRunnable, 0);
        }
        //Do stuff here
    }

    //When we leave this state...
    @Override
    public void onPause() {
        super.onPause();
        //Do stuff here
        secondsTimer.removeCallbacks(secondsTimerRunnable);
    }

    //When we end the activity...
    @Override
    public void onDestroy() {
        super.onDestroy();
        //Do stuff here
    }

    Handler scanningTimer = new Handler();
    Runnable scanningTimerRunnable = new Runnable() {
        @Override
        public void run() {
            mBluetoothAdapter.cancelDiscovery();
            availDeviceText.append(" Click to refresh");
            availDeviceText.setEnabled(true);
        }
    };

    //runs without a timer by reposting this handler at the end of the runnable
    Handler secondsTimer = new Handler();
    Runnable secondsTimerRunnable = new Runnable() {
        @Override
        public void run() {
            secondsTimer.postDelayed(this, 1000);
            millis = System.currentTimeMillis() - startTime;
            int seconds = (int) (millis / 1000);
            int minutes = seconds / 60;
            seconds = seconds % 60;
            timerText.setText(String.format("%02d:%02d", minutes, seconds));
        }
    };

    public void startTimer(View v) {
        if (timerState == 1) {
            timerState = 0;
            secondsTimer.removeCallbacks(secondsTimerRunnable);
        } else {
            timerState = 1;
            startTime = System.currentTimeMillis();
            secondsTimer.postDelayed(secondsTimerRunnable, 0);
        }
    }

    public void listit(){
        pairedDevices = mBluetoothAdapter.getBondedDevices();
        for(BluetoothDevice bt : pairedDevices)
            addItems(pairedDeviceList, bt.getName());
    }

    final BroadcastReceiver bReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            // When discovery finds a device
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                // Get the BluetoothDevice object from the Intent
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                // add the name and the MAC address of the object to the arrayAdapter
                String newDevice = device.getName() + "\n" + device.getAddress();
                if(!availDeviceList.contains(newDevice)) {
                    addItems(availDeviceList, device.getName() + "\n" + device.getAddress());
                    availDeviceAdapter.notifyDataSetChanged();
                }
            }
        }
    };

    //METHOD WHICH WILL HANDLE DYNAMIC LIST INSERTION
    public void addItems(ArrayList someList, String thisItem) {
        someList.add(thisItem);
    }

    public void scanForBluetoothOnClick(View view){
        availDeviceText.setEnabled(false);
        availDeviceText.setText("Available Devices...");
        availDeviceList.clear();
        availDeviceAdapter.notifyDataSetChanged();
        scanForBluetooth();
    }

    private void scanForBluetooth(){

        IntentFilter bluetoothFilter = new IntentFilter();
        bluetoothFilter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        bluetoothFilter.addAction(BluetoothDevice.ACTION_FOUND);
        bluetoothFilter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);

        registerReceiver(bReceiver, bluetoothFilter);

        //cancel any prior BT device discovery
        if (mBluetoothAdapter.isDiscovering()){
            mBluetoothAdapter.cancelDiscovery();
        }

        //check locations permission
        int MY_PERMISSIONS_REQUEST_ACCESS_COARSE_LOCATION = 1;
        ActivityCompat.requestPermissions(this,
                new String[]{Manifest.permission.ACCESS_COARSE_LOCATION},
                MY_PERMISSIONS_REQUEST_ACCESS_COARSE_LOCATION);

        //re-start discovery
        mBluetoothAdapter.startDiscovery();

        //set timer
        scanningTimer.postDelayed(scanningTimerRunnable, SCANNING_TIME);
    }



}