public class SUM_INT {
    public static int TEST(int a, int b)
	{
		
		return a + b;
	}

	public static void main(String[] vars)
	{
	    SUM_INT.TEST(2, 3);
	    int a = 2;
	    int b = 3;
	    int c = SUM_INT.TEST(a, b);
	}
}