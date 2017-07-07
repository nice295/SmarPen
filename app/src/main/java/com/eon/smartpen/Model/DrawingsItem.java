package com.eon.smartpen.Model;

/**
 * Created by kyuholee on 2016. 9. 4..
 */

public class DrawingsItem {

    public DrawingsItem(int time, int x, int y) {
        this.time = time;
        this.x = x;
        this.y = y;
    }

    public int getTime() {
        return time;
    }

    public void setTime(int time) {
        this.time = time;
    }

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
    }

    public int time;
    public int x;
    public int y;

    public DrawingsItem() {
    }


}