public class ADT_test {

	public class TestClass {
		int testA;
		int testB;

		public TestClass(int a)
		{
			testA = a;
			testB = 10;
		}
		}	

		public static boolean eq(int a, int b)
	{
		return a == b;
	}

	public static void main(String[] vars)
	{
	   TestClass test = (new ADT_test()).new TestClass(10);
	   
	   ADT_test.eq(test.testA, test.testB);
	  
	}
}