
// a failed attempt at running multiple threads
class ThreadClient implements Runnable
{
	@Override
	public void run ()
	{

		return ;
	}
	public static void main(String[] args) 
	{
		ThreadClient client = new ThreadClient();  
		Thread thread = new Thread(client);  
		Thread thread_1 = new Thread(client);  
		Thread thread_2 = new Thread(client);  
		thread.start();
		thread_1.start();
		thread_2.start();
		return ;
	}
}