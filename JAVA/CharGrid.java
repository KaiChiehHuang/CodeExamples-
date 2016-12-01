// HW1 2-d array Problems
// CharGrid encapsulates a 2-d grid of chars and supports
// a few operations on the grid.

package assign1;
import  java.util.ArrayList;
import java.util.Collections;

public class CharGrid {
	private char[][] grid;

	/**
	 * Constructs a new CharGrid with the given grid.
	 * Does not make a copy.
	 * @param grid
	 */
	public CharGrid(char[][] grid) {
		this.grid = grid;
	}
	
	/**
	 * Returns the area for the given char in the grid. (see handout).
	 * @param ch char to look for
	 * @return area for given char
	 */
	public int charArea(char ch) {
		ArrayList<Integer> xPositionsOfCha = new ArrayList<Integer>();
		ArrayList<Integer> yPositionsOfCha = new ArrayList<Integer>();
		for (int i = 0; i<grid.length; i++) {
			String charRow = String.valueOf(grid[i]);
			int startCheckPoint = charRow.indexOf(ch,0);
			while (startCheckPoint != -1){
				xPositionsOfCha.add(startCheckPoint);
				yPositionsOfCha.add(i);
				startCheckPoint = charRow.indexOf(ch,startCheckPoint+1);
			}
		}
		// Sort the x and y positions to get min/max x and y to compute Area 
		Collections.sort(xPositionsOfCha);
		Collections.sort(yPositionsOfCha);
		if (!xPositionsOfCha.isEmpty()) {
			return (xPositionsOfCha.get(xPositionsOfCha.size()-1) - xPositionsOfCha.get(0) + 1) * (yPositionsOfCha.get(yPositionsOfCha.size()-1) - yPositionsOfCha.get(0) + 1);
		}
		return 0;
	}
	
	/**
	 * Returns the count of '+' figures in the grid (see handout).
	 * @return number of + in grid
	 */
	public int countPlus() {
		int plusCount = 0;
		for (int i = 1; i<(grid.length-1); i++) {
			for (int j = 1; j<(grid[i].length-1); j++) {
				int armLengthRight = findArmLengthWithDirection(i,j,1,0);
				int armLengthLeft = findArmLengthWithDirection(i,j,-1,0);
				int armLengthUp = findArmLengthWithDirection(i,j,0,-1);
				int armLengthDown = findArmLengthWithDirection(i,j,0,1);
				if (armLengthRight>1 && armLengthRight == armLengthLeft && armLengthLeft == armLengthUp && armLengthUp == armLengthDown) {
					plusCount++;
				}
			}
		}
		return plusCount;
    }
	/**
	 * Returns the count of arm length of certain symbol in the given direction.
	 * Usage: given the position i,j in the grid, this function will find the arm length 
	 * at the given direction starting from the given position in the grid,
	 * with (iIncrement,jIncrement) = (0,1)  -> down
	 * with (iIncrement,jIncrement) = (0,-1) -> up
	 * with (iIncrement,jIncrement) = (1,0)  -> right
	 * with (iIncrement,jIncrement) = (-1,0) -> left
	 */
	private int findArmLengthWithDirection(int i, int j, int iIncrement, int jIncrement) {
		int armLength = 0;
		int iDelta = 0; 
		int jDelta = 0;
		int middleChar = grid[i][j];
		while ( grid[i+iDelta][j+jDelta] == middleChar ) {
			armLength++;
			iDelta = iDelta + iIncrement;
			jDelta = jDelta + jIncrement;
			if ((i+iDelta)>=grid.length || (i+iDelta)<0 || (j+jDelta)>=grid[0].length || (j+jDelta)<0) {
				break;
			}
		}
		return armLength;
	}
}
