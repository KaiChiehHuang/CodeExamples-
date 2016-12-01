package assign1;

import java.util.*;

public class Appearances {
	
	/**
	 * Returns the number of elements that appear the same number
	 * of times in both collections. Static method. (see handout).
	 * @return number of same-appearance elements
	 */
	public static <T> int sameCount(Collection<T> a, Collection<T> b) {
		int numSameCount = 0;
		HashMap<T,Integer> aElementCountMap = new HashMap<T,Integer>();
		HashMap<T,Integer> bElementCountMap = new HashMap<T,Integer>();
		countElementAppearence(a,aElementCountMap);
		countElementAppearence(b,bElementCountMap);
		for (T key : aElementCountMap.keySet()) {
			if (bElementCountMap.containsKey(key)) {
				if (aElementCountMap.get(key) == bElementCountMap.get(key)) {
					numSameCount++;
				}
			}
		}
		return numSameCount; 
	}
	
	private static <T> void countElementAppearence(Collection<T> collection, HashMap<T,Integer> countMap) {
		Iterator<T> itr = collection.iterator();
		while (itr.hasNext()) {
			T current = itr.next();
			if (countMap.get(current) == null) {
				countMap.put(current,1);
			}else {
				countMap.put(current,countMap.get(current)+1);
			}
		}
	}
	
}
