public class CASE_IF_STR {
	public static void main()
	{
		String a = "test";
		String b = "test2";
		if (a.equals(b))
		{
			b = "test3";
		}
		else if (a.equals(b))
		{
			b = "test4";
		}	
		System.out.println(b);
	}
}