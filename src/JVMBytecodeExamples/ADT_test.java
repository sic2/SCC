class TestClass {
		int integer;
		boolean bool;
		String str = "hello";
		TestClass[] classes;
		// int testB;

		// String str;

		public TestClass()
		{
			//testA = 4;
			// testB = 10;
			// str = "hello";
			classes = new TestClass[10];
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
		testONE.integer = 10;
		TestClass testTWO = new TestClass();
		TestClass testTHREE = new TestClass();

		testTWO.classes[0] = testONE;
		testTHREE.classes[0] = testTWO;

	   	System.out.println(testTHREE.classes[0].classes[0].integer);
	}
}