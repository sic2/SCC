public class EQ_INTS {
    public static boolean TEST(int a, int b)
	{
		return a == b;
	}

	public static void main(String[] vars)
	{
	    System.out.println(EQ_INTS.TEST(2, 3));
	    int a = 2;
	    int b = 3;
	    boolean c = EQ_INTS.TEST(a, b);
	    System.out.println(c);
	}
}