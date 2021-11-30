package com.saladmixer.saladmixer;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;

public class browseSalads extends AppCompatActivity {
    private ArrayList<Salad> TEMP_SALADS;
    private saladAdapter ADAPTER;
    RecyclerView recyclerView;
    public static Salad SELECTED_SALAD;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_browse_salads);

        recyclerView = findViewById(R.id.recyclerView);
        ADAPTER = new saladAdapter(this);
        recyclerView.setAdapter(ADAPTER);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        SELECTED_SALAD = null;

        MainActivity.DATAREF = MainActivity.DATABASE.getReference().child(MainActivity.USERID);
        TEMP_SALADS = new ArrayList<>();

        MainActivity.DATAREF.orderByKey().addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                //Read data from user
                TEMP_SALADS = new ArrayList<>();
                for(DataSnapshot salad: dataSnapshot.child("salads").getChildren()){
                    TEMP_SALADS.add(salad.getValue(Salad.class));
                }
                ADAPTER.setSalads(TEMP_SALADS);
            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });


    }
}
