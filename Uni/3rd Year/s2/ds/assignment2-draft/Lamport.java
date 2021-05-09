

// responsible for the lamport clock implmentation
public class Lamport 
{
    // local clock of this process
    private int clock;

    // Constructor
    public Lamport()
    {
        this.clock = -1;
    }

    // Constructor
    public Lamport(int input)
    {
        this.clock = input;
    }

    // given an int array of clocks, return the largest + 1
    public int next_clock (int[] prev_times)
    {
        int new_clock = -1;
        for (int i = 0; i < prev_times.length; ++i)
        {
            if (prev_times[i] > new_clock)
            {
                new_clock = prev_times[i];
            }
        }
        this.clock = new_clock;
        return new_clock;
    }
}