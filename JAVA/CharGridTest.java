// Test cases for CharGrid -- a few basic tests are provided.
package assign1;

import static org.junit.Assert.*;
import org.junit.Test;

public class CharGridTest {
	
	@Test
	public void testCharArea1() {
		char[][] grid = new char[][] {
				{'a', 'y', ' '},
				{'x', 'a', 'z'},
			};
		
		
		CharGrid cg = new CharGrid(grid);
				
		assertEquals(4, cg.charArea('a'));
		assertEquals(1, cg.charArea('z'));
	}
	
	
	@Test
	public void testCharArea2() {
		char[][] grid = new char[][] {
				{'c', 'a', ' '},
				{'b', ' ', 'b'},
				{' ', ' ', 'a'}
			};
		
		CharGrid cg = new CharGrid(grid);
		
		assertEquals(6, cg.charArea('a'));
		assertEquals(3, cg.charArea('b'));
		assertEquals(1, cg.charArea('c'));
	}
	
	// TODO Add more tests
	@Test
	public void testCharArea3() {
		char[][] grid = new char[][] {
			{'a', 'g', 'y'},
			{'b', 'b', 'b'},
			{'y', ' ', 'a'}
		};
		CharGrid cg = new CharGrid(grid);
		
		assertEquals("Test char not appear in grid failed!",0,cg.charArea('f'));
		assertEquals("Test char area equals the whole grid failed!",9,cg.charArea('y'));
		assertEquals("Test char area of ' ' failed!",1,cg.charArea(' '));
	}
	
	@Test
	public void testCountPlusBasic() {
		char[][] grid = new char[][] {
			{'a', 'b', 'y','+','-','d'},
			{'b', 'b', 'b','-','-','-'},
			{'y', 'b', 'a','h','-','@'}
		};
		CharGrid cg = new CharGrid(grid);
		assertEquals(2,cg.countPlus());
		grid = new char[][] {
			{'a', 't', 'b','+','-','d'},
			{'b', 'b', 'b','b','-','-'},
			{'y', '-', 'b','h','-','@'}
		};
		cg = new CharGrid(grid);
		assertEquals(0,cg.countPlus());
	}
	
	@Test
	public void testCountPlusAdvanced() {
		char[][] grid = new char[][] {
			{'a', ' ', 'f','+','-','@'},
			{'b', 'f', 'f','f','@','@'},
			{'y', ' ', 'f','h','-','@'},
			{'y', ' ', 'f','h','-','@'}
		};
		CharGrid cg = new CharGrid(grid);
		assertEquals(0,cg.countPlus());
		grid = new char[][] {
			{'a', 't', '/','+','-','d'},
			{'b', 'b', '/','b','-','-'},
			{'/', '/', '/','/','/','@'},
			{'b', 'b', '/','b','-','-'},
			{'y', '-', '/','h','-','@'}
		};
		cg = new CharGrid(grid);
		assertEquals(1,cg.countPlus());
		grid = new char[][] {
			{'a', 't', '/','+','-','d','g'},
			{'b', 'b', '/','b','-','$','t'},
			{'/', '/', '/','/','/','$','h'},
			{'b', 'b', '/','$','$','$','$'},
			{'y', '-', '/','h','-','$','h'},
			{'/', '/', 'g','/','/','$','s'},
			{'b', 'g', 'g','g','-','-','g'},
			{'y', '-', 'g','h','-','@','q'}
		};
		cg = new CharGrid(grid);
		assertEquals(2,cg.countPlus());
	}
}
