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
	static int A = 0;

	public static void main(String[] vars)
	{


		String a = "hello";
		Object b = a;
		try{
		String c = (String)b;
	} catch (Exception e)
	{
		
	}
		// TestClass testONE = new TestClass();
		// String s = testONE.toString();
		// if (A == 0)
		// {
		// 	System.out.println("int");
		// }
		// if (A == 1)
		// {
		// 	System.out.println("bool");
		// }
		// if (A == 2)
		// {
		// 	System.out.println("str");
		// }
		// if (A == 3)
		// {
		// 	System.out.println("obj");
		// }

		// TestClass testONE = new TestClass();
		// testONE.integer = 10;
		// TestClass testTWO = new TestClass();
		// testTWO.integer = 5;
		// System.out.println(testTWO.compare(testONE));

		// System.out.println("hello");
	}
}