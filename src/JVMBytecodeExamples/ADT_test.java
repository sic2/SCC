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

		public boolean compare(TestClass obj)
		{	
			if (integer == obj.integer)
				if (str.equals(obj.str))
					return true;
			
			return false;
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
		testTWO.integer = 5;
		System.out.println(testTWO.compare(testONE));

		System.out.println("hello");
	}
}