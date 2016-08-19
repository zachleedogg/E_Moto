package com.zacharylevenberg.testapp;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.EditText;

public class WelcomeScreen extends AppCompatActivity {
    //Create a key to pair with message for the Intent
    public final static String EXTRA_MESSAGE = "com.zacharylevenberg.testapp.MESSAGE";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_welcome_screen);
    }

    ////When we arrive at this state again some time...
    @Override
    public void onResume() {
        super.onResume();
        ((EditText) findViewById(R.id.user_id)).setText("");
    }

    //When we leave this state...
    @Override
    public void onPause() {
        super.onPause();
        ((EditText) findViewById(R.id.user_id)).setText("");
    }

    /** Called when the user clicks the Send button */
    public void sendMessage(View view) {
        //Create a new Intent
        Intent intent = new Intent(this, HomeScreen.class);
        //Get text from textbox
        EditText editText = (EditText) findViewById(R.id.user_id);
        //convert it to a string
        String message = editText.getText().toString();
        //add the string to the intent
        intent.putExtra(EXTRA_MESSAGE, message);
        //start a new activity
        startActivity(intent);

    }
}
