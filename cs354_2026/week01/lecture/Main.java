import java.util.List;
import java.util.ArrayList;
import java.util.LinkedList;

public class Main {
    
    public static void insert(List<Integer> list) {
	for(int i=0;i<Main.BIG_SIZE;i++)
	    list.add(0,i);
    }

    public static void displaySum(List<Integer> list) {
	int sum = 0;
	for(int i=0;i<Main.BIG_SIZE;i++)
	    sum += list.get(i);	
    }

    public static int BIG_SIZE = 99999;
    
    public static void test(List<Integer> list) {
        long start = System.nanoTime();
        Main.insert(list);
        long mid = System.nanoTime();
        Main.displaySum(list);
        long end = System.nanoTime();
        
        System.out.println(list.getClass().getName());
        System.out.println("insert(): "+(mid-start)/(double)BIG_SIZE);
        System.out.println("displaySum(): "+(end-mid)/(double)BIG_SIZE);
    }
    
    public static void main(String[] args) {
	    Main.test(new ArrayList<>());
        Main.test(new LinkedList<>()); 
    }
}
