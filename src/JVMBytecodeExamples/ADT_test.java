class TestClass {
		int integer;
		boolean bool;
		String str;
		TestClass[] classes;
		// int testB;

		// String str;

		public TestClass()
		{
			//testA = 4;
			// testB = 10;
			// str = "hello";
			//classes = new TestClass[10];
		}	

	}

public class ADT_test {

	// public static boolean eq(int a, int b)
	// {
	// 	return a == b;
	// }

	public static void main(String[] vars)
	{
	  TestClass testONE = new TestClass();
	  //testONE.integer = 10;
	  // testONE.str = "hello";
	 TestClass testTWO = new TestClass();
	 testTWO.integer = 10;

	 testONE.classes = new TestClass[10];

	 testONE.classes[0] = testTWO;

	 int test = testONE.classes[0].integer;

	// testTWO.classes[0] = testONE;

	   
	  //System.out.println(ADT_test.eq(test.testA, test.testB));
	  
	}
}