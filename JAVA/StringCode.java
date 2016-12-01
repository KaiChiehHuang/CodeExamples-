package assign1;

import java.util.HashSet;

// CS108 HW1 -- String static methods

public class StringCode {

	/**
	 * Given a string, returns the length of the largest run.
	 * A a run is a series of adjacent chars that are the same.
	 * @param str
	 * @return max run length
	 */
	public static int maxRun(String str) {
		if (str.isEmpty()) {
			return 0;
		}
		int maxRun = 0;
		int runCount = 0;
		char pre = str.charAt(0);
		for (int i = 0; i<str.length(); i++) {
			if (str.charAt(i) == pre) {
				runCount++;
			}else {
				runCount = 1;
				pre = str.charAt(i);
			}
			if (runCount > maxRun) {
				maxRun = runCount;
			}
		}
		return maxRun;
	}

	
	/**
	 * Given a string, for each digit in the original string,
	 * replaces the digit with that many occurrences of the character
	 * following. So the string "a3tx2z" yields "attttxzzz".
	 * @param str
	 * @return blown up string
	 */
	public static String blowup(String str) {
		String blownUpString = "";
		for (int i = 0; i<str.length(); i++) {
			if (Character.isDigit(str.charAt(i))) {
				if (i+1<str.length()) {
					for (int j = 0; j<Character.getNumericValue(str.charAt(i)); j++) {
						blownUpString = blownUpString + str.charAt(i+1);
					}
				}
			}else {
				blownUpString = blownUpString + str.charAt(i);
			}
		}
		return blownUpString;
	}
	
	/**
	 * Given 2 strings, consider all the substrings within them
	 * of length len. Returns true if there are any such substrings
	 * which appear in both strings.
	 * Compute this in linear time using a HashSet. Len will be 1 or more.
	 */
	public static boolean stringIntersect(String a, String b, int len) {
		HashSet<String> aSubStrings = new HashSet<String>();
		HashSet<String> bSubStrings = new HashSet<String>();
		for (int i = 0; i< Math.max(a.length(),b.length()); i++) { 
			if (i+len<=a.length()) {
				aSubStrings.add(a.substring(i, i+len));
			}
			if (i+len<=b.length()) {
				bSubStrings.add(b.substring(i, i+len));
			}
		}
		int numTotalSubStrings = aSubStrings.size() + bSubStrings.size();
		aSubStrings.addAll(bSubStrings);
		if (aSubStrings.size()<numTotalSubStrings) {
			return true;
		}
		return false; 
	}
}
