// TabooTest.java
// Taboo class tests -- nothing provided.
package assign1;

import static org.junit.Assert.*;
import org.junit.Test;
import java.util.*;

public class TabooTest {

	// TODO ADD TESTS
	@Test
	public void testTabooNoFollowBasic() {
		// Testing normal rules
		List<Character> rules = Arrays.asList('a','b','a','t','y');
		Taboo<Character> taboo = new Taboo<Character>(rules);
		Set<Character> noFollowSet = new HashSet<Character>(Arrays.asList('b','t'));
		assertTrue(noFollowSet.equals(taboo.noFollow('a')));
		// Testing another element 
		rules = Arrays.asList('a','b','c','a','b','y');
		taboo = new Taboo<Character>(rules);
		noFollowSet = new HashSet<Character>(Arrays.asList('c','y'));
		assertTrue(noFollowSet.equals(taboo.noFollow('b')));
	}
	
	@Test
	public void testTabooNoFollowAdvanced() {
		// Testing rules with null
		List<Character>	 rules = Arrays.asList('a','b',null,'a','y');
		Taboo<Character> taboo = new Taboo<Character>(rules);
		Set<Character>	 noFollowSet = new HashSet<Character>(Arrays.asList('b','y'));
		assertTrue(noFollowSet.equals(taboo.noFollow('a')));
		// Testing element that follows null in rules 
		rules = Arrays.asList('a','b',null,'a','y');
		taboo = new Taboo<Character>(rules);
		noFollowSet = Collections.emptySet();
		assertTrue(noFollowSet.equals(taboo.noFollow('b')));
		// Testing element not appear in rules 
		rules = Arrays.asList('a','b','c','a','y');
		taboo = new Taboo<Character>(rules);
		noFollowSet = Collections.emptySet();
		assertTrue(noFollowSet.equals(taboo.noFollow('x')));
		// Testing element in the end
		rules = Arrays.asList('a','b','c','a','y');
		taboo = new Taboo<Character>(rules);
		noFollowSet = Collections.emptySet();
		assertTrue(noFollowSet.equals(taboo.noFollow('y')));
	}
	
	@Test
	public void testTabooReduceBasic() {
		// Testing normal reduction 
		List<Character> rules = Arrays.asList('a','b','a','t','y');
		Taboo<Character> taboo = new Taboo<Character>(rules);
		List<Character>  listToReduce = new ArrayList<Character>(Arrays.asList('a','b','a','t','g'));
		List<Character>  listAfterReduce = Arrays.asList('a','a','g');
		taboo.reduce(listToReduce);
		assertTrue(listAfterReduce.equals(listToReduce));
		// Testing consecutive reduction 
		rules = Arrays.asList('a','b','a','t','y');
		taboo = new Taboo<Character>(rules);
		listToReduce = new ArrayList<Character>(Arrays.asList('a','b','t','f','g'));
		listAfterReduce = Arrays.asList('a','f','g');
		taboo.reduce(listToReduce);
		assertTrue(listAfterReduce.equals(listToReduce));
	}
	@Test
	public void testTabooReduceAdvanced() {
		// Testing empty rule
		List<Character> rules = Collections.emptyList();
		Taboo<Character> taboo = new Taboo<Character>(rules);
		List<Character>  listToReduce = new ArrayList<Character>(Arrays.asList('a','b','a','t','y'));
		List<Character>  listAfterReduce = new ArrayList<Character>(Arrays.asList('a','b','a','t','y'));
		taboo.reduce(listToReduce);
		assertTrue(listAfterReduce.equals(listToReduce));
		// Testing rules with null
		rules = Arrays.asList('a','b',null,'t','y');
		taboo = new Taboo<Character>(rules);
		listToReduce = new ArrayList<Character>(Arrays.asList('a','b','t','y','g'));
		listAfterReduce = Arrays.asList('a','t','g');
		taboo.reduce(listToReduce);
		assertTrue(listAfterReduce.equals(listToReduce));
		// Testing related rules 
		rules = Arrays.asList('y','b','y','t','y');
		taboo = new Taboo<Character>(rules);
		listToReduce = new ArrayList<Character>(Arrays.asList('b','y','t','y','b','t'));
		listAfterReduce = Arrays.asList('b','t','b','t');
		taboo.reduce(listToReduce);
		assertTrue(listAfterReduce.equals(listToReduce));
	}
}
