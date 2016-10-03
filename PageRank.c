// #####################
// SIMPLE PAGE RANK ALGO
// #####################

int PageRank(myPage) {
	int rank = 0;
	for (each page in the web) {
		if (linkExists(page,myPage))
			rank++;
	}
	return rank;
}

// #################
// GOOGLE'S SOLUTION
// #################

// This algo never explicity represents the graph
// (otherwise it won't be feasible to maintain such a list)
void pageRank(randomPage) {
	curr = randomPage;
	for (a long time) {
		if (!isElem(seen, (prev,curr)))					// If page never seen before: (somewhere is a rep for seen edges)
			if (!inArray(rank,curr)) rank[curr] = 0;		// An array of ranks. Set rank = 0 for never seen before pages
			rank[curr]++;									// Otherwise, increment rank if seen before
			seen = include[seen, (prev,curr)];				// Include in list of seen edges
		if (random(0,100) < 85)							// Choose a hyperlink to visit in curr page
			prev = curr;									// Set prev as current page
			curr = choose hyperlink from curr;				// Set curr page as new hyperlink page
		else
			// avoids getting stuck
			prev = NULL;								// Choose another random page, rather than follow link
			curr = randomPage;							// from current page. Randomly jump around page.s
	}
}


