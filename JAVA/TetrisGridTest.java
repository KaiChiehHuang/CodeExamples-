package assign1;

import static org.junit.Assert.*;
import org.junit.Test;

import java.util.*;

public class TetrisGridTest {
	
	// Provided simple clearRows() test
	// width 2, height 3 grid
	@Test
	public void testClear1() {
		boolean[][] before =
		{	
			{true, true, false, },
			{false, true, true, }
		};
		
		boolean[][] after =
		{	
			{true, false, false},
			{false, true, false}
		};
		
		TetrisGrid tetris = new TetrisGrid(before);
		tetris.clearRows();
		assertTrue( Arrays.deepEquals(after, tetris.getGrid()) );
		
		before = new boolean[][]
		{	
			{true, true, true, },
			{true, true, true, },
			{true, false,true}
		};
			
		after = new boolean[][]
		{	
			{true, false, false},
			{true, false, false},
			{false, false,false}
		};
		tetris = new TetrisGrid(before);
		tetris.clearRows();
		assertTrue( Arrays.deepEquals(after, tetris.getGrid()) );
	}
	
	// TODO ADD TESTS
	@Test
	public void testClearAdvanced() {
		boolean[][] before = new boolean[][] {
			{true,true,true,false},
			{true,true,false,true},
			{true,true,false,true}
		}; 
		boolean[][] after = new boolean[][] {
			{true,false,false,false},
			{false,true,false,false},
			{false,true,false,false}
		}; 
		TetrisGrid tetris = new TetrisGrid(before);
		tetris.clearRows();
		assertTrue(Arrays.deepEquals(after, tetris.getGrid()));
		// Test full row not adjacent 
		before = new boolean[][] {
			{true,true,true,false},
			{true,false,true,true},
			{true,true,true,true}
		}; 
		after = new boolean[][] {
			{true,false,false,false},
			{false,true,false,false},
			{true,true,false,false}
		}; 
		tetris = new TetrisGrid(before);
		tetris.clearRows();
		assertTrue(Arrays.deepEquals(after, tetris.getGrid()));
		// Test full row in the end
		before = new boolean[][] {
			{true,true,true,true},
			{false,false,true,true},
			{true,true,true,true},
			{false,true,true,true}
		}; 
		after = new boolean[][] {
			{true,true,false,false},
			{false,false,false,false},
			{true,true,false,false},
			{false,true,false,false}
		}; 
		tetris = new TetrisGrid(before);
		tetris.clearRows();
		assertTrue(Arrays.deepEquals(after, tetris.getGrid()));
	}
	
}
