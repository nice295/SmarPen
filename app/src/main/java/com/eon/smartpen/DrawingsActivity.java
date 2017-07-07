package com.eon.smartpen;

import android.content.Intent;
import android.graphics.Color;
import android.graphics.Path;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.animation.AccelerateDecelerateInterpolator;

import com.eftimoff.androipathview.PathView;
import com.eon.smartpen.Model.DrawingsIndexItem;
import com.eon.smartpen.Model.DrawingsItem;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.Query;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;


public class DrawingsActivity extends AppCompatActivity {

    private static final String TAG = "DrawingsActivity";
    private DatabaseReference mDatabase;
    private ArrayList<DrawingsItem> mMyItemArray;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_drawing);

        Intent intent = getIntent();
        int index = intent.getExtras().getInt("index");
        Log.d(TAG, "Index: " + index);

        mMyItemArray = new ArrayList<DrawingsItem>();

        mDatabase = FirebaseDatabase.getInstance().getReference();

        Query myTopPostsQuery = mDatabase.child("words").child(String.valueOf(index));
        myTopPostsQuery.addValueEventListener(
                new ValueEventListener() {
                    @Override
                    public void onDataChange(DataSnapshot dataSnapshot) {

                        mMyItemArray.clear();

                        if (dataSnapshot.getChildrenCount() != 0) {
                            for (DataSnapshot postSnapshot : dataSnapshot.getChildren()) {
                                DrawingsItem item = postSnapshot.getValue(DrawingsItem.class);

                                Log.d(TAG, "Time: " + item.getTime());
                                Log.d(TAG, "X: " + item.getX());
                                Log.d(TAG, "Y: " + item.getY());

                                mMyItemArray.add(item);
                            }
                            drawText();
                         }
                    }

                    @Override
                    public void onCancelled(DatabaseError databaseError) {
                        Log.w(TAG, "words:onCancelled", databaseError.toException());
                    }
                }
        );
    }

    private void drawText() {

        final PathView pathView = (PathView) findViewById(R.id.pathView);

        final Path path = makeConvexArrow(50, 100);
        pathView.setPath(path);
        pathView.setFillAfter(true);
        pathView.setPathColor(Color.BLACK);

        pathView.getSequentialPathAnimator()
                .delay(100)
                .duration(500)
                .interpolator(new AccelerateDecelerateInterpolator())
                .start();
    }


    private Path makeConvexArrow(float length, float height) {
        final Path path = new Path();
        for (int idx = 0; idx < mMyItemArray.size()-1; idx++) {
            path.moveTo(mMyItemArray.get(idx).getX(), mMyItemArray.get(idx).getY());
            path.lineTo(mMyItemArray.get(idx+1).getX(), mMyItemArray.get(idx+1).getY());
        }

        path.close();
        return path;
    }
}
