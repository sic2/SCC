public class CASE_INTS {
	public static void main()
	{
		int a = 0;
		int b = 1;
		switch(a)
		{
			case 0:
			b = 1;
			break;
			case 1:
			b = 0;
			break;
			default:
			break;
		}

		System.out.println(b);
	}
}