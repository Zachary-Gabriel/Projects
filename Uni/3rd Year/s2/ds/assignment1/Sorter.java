import java.rmi.Remote; 
import java.rmi.RemoteException;  
import java.io.Serializable;

// exception handling or something (I'm not an expert)
public interface Sorter extends Remote 
{  
	// pushes value onto the stack
	void push_value (int val) throws RemoteException;

	// push operation to change the stack
	void push_operator (String operator) throws RemoteException;

	// pops a variable from the stack
	int pop () throws RemoteException;

	// returns true if the stack is empty and false otherwise
	boolean is_empty () throws RemoteException;

	// pops a variable from the stack after "millis" milliseconds
	int delay_pop (int millis) throws RemoteException;
}