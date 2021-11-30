package com.saladmixer.saladmixer;

import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import androidx.recyclerview.widget.RecyclerView;
import java.util.List;
import java.util.Random;


public class saladAdapter extends RecyclerView.Adapter<saladAdapter.saladViewHolder> {

    class saladViewHolder extends RecyclerView.ViewHolder {
        private final TextView saladItemView;
        private final Button buttonRemove;
        private final Button buttonView;
        private final ImageView imageView;

        private saladViewHolder(View itemView) {
            super(itemView);
            saladItemView = itemView.findViewById(R.id.textView);
            buttonRemove = itemView.findViewById(R.id.buttonRemove);
            buttonView = itemView.findViewById(R.id.buttonView);
            imageView = itemView.findViewById(R.id.imageView);
        }
    }

    private final LayoutInflater mInflater;
    private List<Salad> salads;
    private Context mContext;


    public saladAdapter(Context context) {
        mInflater = LayoutInflater.from(context);
        mContext = context;
    }

    @Override
    public saladViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = mInflater.inflate(R.layout.recyclerview_saladitem, parent, false);
        return new saladViewHolder(itemView);
    }

    @Override
    public void onBindViewHolder(saladViewHolder holder, int position) {
        if (salads != null) {
            final Salad current = salads.get(position);
            final String key = current.getName();
            String title = current.getIngredient1().getName() + "-\n"
                    + current.getIngredient2().getName() + "-\n"
                    + current.getIngredient3().getName();

            holder.saladItemView.setText(title);
            //Set random image
            Random rand = new Random();
            int randInt = rand.nextInt(7);
            holder.imageView.setImageResource(randomImage(randInt));
            holder.buttonRemove.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    MainActivity.DATABASE.getReference().child(MainActivity.USERID)
                            .child("salads").child(key).removeValue();
                }
            });
            holder.buttonView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    browseSalads.SELECTED_SALAD = current;
                    Intent intent = new Intent(mContext, saladInfo.class);
                    mContext.startActivity(intent);
                }
            });

        } else {
            // Covers the case of data not being ready yet.
            holder.saladItemView.setText("");
            holder.buttonRemove.setVisibility(View.INVISIBLE);
            holder.buttonView.setVisibility(View.INVISIBLE);
        }
    }

    void setSalads(List<Salad> tempSalads){
        salads = tempSalads;
        notifyDataSetChanged();
    }

    @Override
    public int getItemCount() {
        if (salads != null)
            return salads.size();
        else return 0;
    }

     private int randomImage(int random){
        if(random == 0){
            return R.mipmap.salad1;
        }
        else if(random == 1){
            return R.mipmap.salad2;
        }
        else if(random == 2){
            return R.mipmap.salad3;
        }
        else if(random == 3){
            return R.mipmap.salad4;
        }
        else if(random == 4){
            return R.mipmap.salad5;
        }
        else if(random == 5){
            return R.mipmap.salad6;
        }
        else{
            return R.mipmap.salad_main;
        }
    }
}
