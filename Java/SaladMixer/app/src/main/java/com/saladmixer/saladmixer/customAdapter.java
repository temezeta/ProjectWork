package com.saladmixer.saladmixer;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import androidx.appcompat.app.AlertDialog;
import androidx.recyclerview.widget.RecyclerView;
import java.util.List;


public class customAdapter extends RecyclerView.Adapter<customAdapter.ingredientViewHolder> {

    class ingredientViewHolder extends RecyclerView.ViewHolder {
        private final TextView ingredientItemView;
        private final Button buttonRemove;
        private final Button buttonView;

        private ingredientViewHolder(View itemView) {
            super(itemView);
            ingredientItemView = itemView.findViewById(R.id.textView);
            buttonRemove = itemView.findViewById(R.id.buttonRemove);
            buttonView = itemView.findViewById(R.id.buttonView);
        }
    }

    private final LayoutInflater mInflater;
    private List<Ingredient> ingredients; // Cached copy of ingredients
    private Context mContext;

    public customAdapter(Context context) {
        mInflater = LayoutInflater.from(context);
        mContext = context;
    }

    @Override
    public ingredientViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = mInflater.inflate(R.layout.recyclerview_carditem, parent, false);
        return new ingredientViewHolder(itemView);
    }

    @Override
    public void onBindViewHolder(ingredientViewHolder holder, int position) {
        if (ingredients != null) {
            final Ingredient current = ingredients.get(position);
            final String key = current.getName();
            holder.ingredientItemView.setText(current.getName());
            holder.buttonRemove.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    MainActivity.DATABASE.getReference().child(MainActivity.USERID)
                            .child("ingredients").child(key).removeValue();
                }
            });
            holder.buttonView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    BrowseIngredients.SELECTED_INGREDIENT = current;
                    Intent intent = new Intent(mContext, ingredientInfo.class);
                    mContext.startActivity(intent);
                }
            });
        } else {
            // Covers the case of data not being ready yet.
            holder.ingredientItemView.setText("");
            holder.buttonRemove.setVisibility(View.INVISIBLE);
            holder.buttonView.setVisibility(View.INVISIBLE);
        }
    }

    void setIngredients(List<Ingredient> tempIngredients){
        ingredients = tempIngredients;
        notifyDataSetChanged();
    }

    // getItemCount() is called many times, and when it is first called,
    // ingredients has not been updated (means initially, it's null, and we can't return null).
    @Override
    public int getItemCount() {
        if (ingredients != null)
            return ingredients.size();
        else return 0;
    }
}
