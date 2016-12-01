// StringCodeTest
// Some test code is provided for the early HW1 problems,
// and much is left for you to add.

package assign1;

import static org.junit.Assert.*;
import org.junit.Test;

public class StringCodeTest {
	//
	// blowup
	//
	@Test
	public void testBlowup1() {
		// basic cases
		assertEquals("xxaaaabb", StringCode.blowup("xx3abb"));
		assertEquals("xxxZZZZ", StringCode.blowup("2x3Z"));
	}
	
	@Test
	public void testBlowup2() {
		// things with digits
		
		// digit at end
		assertEquals("axxx", StringCode.blowup("a2x3"));
		
		// digits next to each other
		assertEquals("a33111", StringCode.blowup("a231"));
		
		// try a 0
		assertEquals("aabb", StringCode.blowup("aa0bb"));
	}
	
	@Test
	public void testBlowup3() {
		// weird chars, empty string
		assertEquals("AB&&,- ab", StringCode.blowup("AB&&,- ab"));
		assertEquals("", StringCode.blowup(""));
		
		// string with only digits
		assertEquals("", StringCode.blowup("2"));
		assertEquals("33", StringCode.blowup("23"));
	}
	
	
	//
	// maxRun
	//
	@Test
	public void testRun1() {
		assertEquals(1,StringCode.maxRun("ab"));
		assertEquals(2, StringCode.maxRun("hoopla"));
		assertEquals(3, StringCode.maxRun("hoopllla"));
	}
	
	@Test
	public void testRun2() {
		assertEquals(3, StringCode.maxRun("abbcccddbbbxx"));
		assertEquals(0, StringCode.maxRun(""));
		assertEquals(3, StringCode.maxRun("hhhooppoo"));
	}
	
	@Test
	public void testRun3() {
		// "evolve" technique -- make a series of test cases
		// where each is change from the one above.
		assertEquals(1, StringCode.maxRun("123"));
		assertEquals(2, StringCode.maxRun("1223"));
		assertEquals(2, StringCode.maxRun("112233"));
		assertEquals(3, StringCode.maxRun("1112233"));
	}

	// TODO Need test cases for stringIntersect
	@Test
	public void testIntersectBasic() {
		// 2 Strings with equal length
		assertEquals(true,StringCode.stringIntersect("ieatyou", "hteatme", 3));
		// 2 Strings with different length
		assertEquals(true,StringCode.stringIntersect("ieatyoueurj", "hteatme", 3));
		// Try different subString length
		assertEquals(true,StringCode.stringIntersect("ieatyou", "hteatme", 2));
		// Try a false result 
		assertEquals(false,StringCode.stringIntersect("abjfiet", "mnckorit", 3));
	}
	
	@Test
	public void testIntersectAdvanced() {
		// 2 Strings with some capital letter
		assertEquals(true,StringCode.stringIntersect("iEatyou", "htEatme", 3));
		assertEquals(false,StringCode.stringIntersect("ieAtyou", "hteatme", 3));
		
		// More than one intersect subStrings
		assertEquals(true,StringCode.stringIntersect("ieatyou", "pyoueat", 2));
	}
	
	@Test
	public void testIntersectEdge() {
		// With empty strings
		assertEquals(false,StringCode.stringIntersect("ieatyou", "", 3));
		assertEquals(false,StringCode.stringIntersect("", "", 3));
		// With weird char
		assertEquals(true,StringCode.stringIntersect("iea%$##", "%$##FGirht", 4));
	}
}
