//
// TetrisGrid encapsulates a tetris board and has
// a clearRows() capability.
package assign1;

public class TetrisGrid {
	
	private boolean[][] tetrisGrid;
	/**
	 * Constructs a new instance with the given grid.
	 * Does not make a copy.
	 * @param grid
	 */
	public TetrisGrid(boolean[][] grid) {
		this.tetrisGrid = grid;
	}
	
	
	/**
	 * Does row-clearing on the grid (see handout).
	 */
	public void clearRows() {
		int y = 0;
		while (y<tetrisGrid[0].length) {
			boolean isFullRow = true;
			for (int x = 0; x<tetrisGrid.length; x++) {
				isFullRow = isFullRow && tetrisGrid[x][y];
			}
			if (isFullRow) {
				for (int x = 0; x<tetrisGrid.length; x++) {
					if (y != (tetrisGrid[0].length-1)) {
						System.arraycopy(tetrisGrid[x], y+1, tetrisGrid[x], y, tetrisGrid[0].length-y-1); 
					}
					tetrisGrid[x][tetrisGrid[0].length-1] = false;
				}
			}else {
				y++;
			}
		}
	}
	
	/**
	 * Returns the internal 2d grid array.
	 * @return 2d grid array
	 */
	boolean[][] getGrid() {
		return tetrisGrid; // TODO YOUR CODE HERE
	}
}
