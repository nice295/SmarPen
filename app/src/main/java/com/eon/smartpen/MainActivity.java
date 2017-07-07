package com.eon.smartpen;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

import com.eon.smartpen.Model.DrawingsIndexItem;
import com.eon.smartpen.Model.DrawingsItem;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.Query;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;


public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    private ListView mLvMyItems;
    private ListViewAdapter mAdapter = null;
    private ArrayList<DrawingsIndexItem> mMyItemArray;
    private DatabaseReference mDatabase;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mLvMyItems = (ListView) findViewById(R.id.lvDrawings);
        mMyItemArray = new ArrayList<DrawingsIndexItem>();
        mAdapter = new ListViewAdapter(this, android.R.layout.simple_list_item_1, mMyItemArray);
        mLvMyItems.setAdapter(mAdapter);

        mDatabase = FirebaseDatabase.getInstance().getReference();

        Query myTopPostsQuery = mDatabase.child("wordsIndex");
        myTopPostsQuery.addValueEventListener(
                new ValueEventListener() {
                    @Override
                    public void onDataChange(DataSnapshot dataSnapshot) {
                        mMyItemArray.clear();

                        if (dataSnapshot.getChildrenCount() != 0) {
                            for (DataSnapshot postSnapshot : dataSnapshot.getChildren()) {
                                DrawingsIndexItem item = postSnapshot.getValue(DrawingsIndexItem.class);
                                mMyItemArray.add(item);

                                Log.d(TAG, "Loading Index: " + item.getIndex());
                            }
                            mAdapter.notifyDataSetChanged();
                        }
                    }

                    @Override
                    public void onCancelled(DatabaseError databaseError) {
                        Log.w(TAG, "words:onCancelled", databaseError.toException());
                    }
                }
        );

        mLvMyItems.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                DrawingsIndexItem item = mAdapter.getItem().get(i);

                Intent intent = new Intent(getApplicationContext(), DrawingsActivity.class);
                intent.putExtra("index", item.getIndex());
                // words/<index>/...

                startActivity(intent);
            }
        });
    }

    private class ListViewAdapter extends ArrayAdapter<DrawingsIndexItem> {
        private ArrayList<DrawingsIndexItem> items;

        public ListViewAdapter(Context context, int textViewResourceId, ArrayList<DrawingsIndexItem> items) {
            super(context, textViewResourceId, items);
            this.items = items;
        }

        public class ViewHolder {
            public TextView tvTime;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            ListViewAdapter.ViewHolder viewHolder;

            Log.d(TAG, "position: " + position);

            DrawingsIndexItem item = items.get(position);

            if (convertView == null) {
                LayoutInflater vi = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                convertView = vi.inflate(R.layout.layout_item_list_item, parent, false);

                viewHolder = new ListViewAdapter.ViewHolder();
                viewHolder.tvTime = (TextView) convertView.findViewById(R.id.tvTime);

                convertView.setTag(viewHolder);
            }
            else {
                viewHolder = (ListViewAdapter.ViewHolder) convertView.getTag();

            }

            Log.d(TAG, "Time: " + item.getIndex());
            viewHolder.tvTime.setText(String.valueOf(item.getIndex()));

            return convertView;
        }

        public ArrayList<DrawingsIndexItem> getItem() {
            return this.items;
        }
    }
}
