package client;

public class ConfigElement{
    private int line;
    private int[] interval;

    public ConfigElement(int setLine, int intBeg, int intEnd){
        this.line = setLine;
        this.interval = new int[2];
        this.interval[0] = intBeg;
        this.interval[1] = intEnd;
    }

    public ConfigElement(String parameterName){
        if (parameterName == "addr"){
            this.line = 2;
            this.interval = new int[2];
            this.interval[0] = 21;
            this.interval[1] = 34;
        }
        else if (parameterName == "id"){
            this.line = 5;
            this.interval = new int[2];
            this.interval[0] = 5;
            this.interval[1] = 7;
        }
        else if (parameterName == "port"){
            this.line = 8;
            this.interval = new int[2];
            this.interval[0] = 18;
            this.interval[1] = 23;
        }
        else if (parameterName == "ping"){
            this.line = 11;
            this.interval = new int[2];
            this.interval[0] = 24;
            this.interval[1] = 26;
        }
        else if (parameterName == "visu"){
            this.line = 14;
            this.interval = new int[2];
            this.interval[0] = 12;
            this.interval[1] = -1;
        }
    }

    public int getLine(){
        return this.line;
    }

    public int[] getInterval(){
        int[] res = new int[this.interval.length];
        for (int i = 0; i < this.interval.length ; i++){
            res[i] = this.interval[i];
        }
        return res;
    }

    @Override
    public String toString(){
        System.out.println(this.line);
        for (int i = 0; i < this.interval.length ; i++){
            System.out.println(this.interval[i]);
        }
        return "";
    }
}
