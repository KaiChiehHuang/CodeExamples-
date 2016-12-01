/*
 HW1 Taboo problem class.
 Taboo encapsulates some rules about what objects
 may not follow other objects.
 (See handout).
*/
package assign1;

import java.util.*;

public class Taboo<T> {
	
	private List<T> rules;
	/**
	 * Constructs a new Taboo using the given rules (see handout.)
	 * @param rules rules for new Taboo
	 */
	public Taboo(List<T> rules) {
		this.rules = rules;
	}
	
	/**
	 * Returns the set of elements which should not follow
	 * the given element.
	 * @param elem
	 * @return elements which should not follow the given element
	 */
	public Set<T> noFollow(T elem) {
		 Set<T> noFollowSet = new HashSet<T>();
		 Iterator<T> iter = rules.iterator();
		 while(iter.hasNext()) {
			 T current = iter.next();
			 if (current != null && current.equals(elem)) {
				 if (iter.hasNext()) {
					 T noFollowElem = iter.next();
					 if (noFollowElem != null) {
						 noFollowSet.add(noFollowElem);
					 }
				 }
			 }
		 }
		 return noFollowSet;
	}
	
	/**
	 * Removes elements from the given list that
	 * violate the rules (see handout).
	 * @param list collection to reduce
	 */
	public void reduce(List<T> list) {
		int i = 0;
		while (i < list.size()) {
			T current = list.get(i);
			Set<T> noFollowSet = this.noFollow(current);
			if (i+1<list.size()) {
				while(noFollowSet.contains(list.get(i+1))){
					list.remove(i+1);
				} 
			}
			i++;
		}
	}
}
