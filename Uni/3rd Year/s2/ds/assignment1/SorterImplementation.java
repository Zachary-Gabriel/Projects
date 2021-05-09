//import java.stack thing
import java.io.Serializable;
import java.util.*;

// Implementing the interface method 
public class SorterImplementation implements Sorter
{  
	private Stack<Integer> stack = new Stack<Integer>();

	// pushes value onto the stack
	public void push_value (int val)
	{
		System.out.println ("pushing val: " + val + " to stack.");

		stack.push (val);
		return ;
	}

	// pushes operator to change the stack
	public void push_operator (String operator)
	{
		int i = 0;
		int[] array = new int[stack.size()];

		while ( 0 < stack.size())
		{ 
			array[i] = stack.pop ();
			++i;
		}
		Arrays.sort (array);

		switch (operator)
		{
			case "ascending":
			System.out.println ("switch triggered: ascending");

			// pushing in ascending order
			for (i = array.length-1; i >= 0; --i)
			{
				stack.push (array[i]);
			}

			// freeing memory
			array = null;
			break;

			case "descending":
			System.out.println ("switch triggered: descending");

			// pushing in ascending order
			for (i = 0; i < array.length; ++i)
			{
				stack.push (array[i]);
			}
			
			// freeing memory
			array = null;
			break;

			case "max":
				System.out.println ("switch triggered: max");

			// push min if there's elements
			if (array.length > 0)
				stack.push (array[array.length-1]);
			break;

			case "min":
			System.out.println ("switch triggered: min");

			// push min if there's elements
			if (array.length > 0)
				stack.push (array[0]);
			break;
		}
		return ;
	}

	// pops value from the stack
	public int pop ()
	{
		int ret = (int) stack.pop ();

		System.out.println ("popping : " + ret + " from the stack.");
		return ret;
	}

	// returns true if the stack is empty and flase otherwise
	public boolean is_empty ()
	{
		if (stack.size() == 0)
		{
			System.out.println ("is empty: true");
			return true;
		}

		System.out.println ("is empty: false");
		return false;
	}

	// after "millis" milliseconds, pops a value from the stack
	public int delay_pop (int millis)
	{
		// halts using sleep
		try
		{
			Thread.sleep (millis);
		}
		catch (Exception e)
		{
			System.err.println ("Server exception: " + e.toString ()); 
			e.printStackTrace ();
		}		
		
		// pops value from stack
		int ret = (int) stack.pop ();
		
		// for debugging
		System.out.println ("popping : " + ret + " from the stack after :" + millis + " ms.");

		return ret;
	} 
} 