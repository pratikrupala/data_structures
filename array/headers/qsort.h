#define SWAP(a, b, t) ((t = a), (a = b), (b = t))

int do_partition(int a[], int start, int end)
{
        int pivot = end;
        int limit = start - 1;
        int forward = start;
        int tmp = 0;
 
 	for (forward = start; forward < end; forward++) {
                if ((a[pivot] >= a[forward])) {
	                limit++;
                        SWAP(a[forward], a[limit], tmp);
        	}
        }

        SWAP(a[pivot], a[limit + 1], tmp);
        return limit + 1;
}

void my_qsort(int a[], int start, int end)
{
	if (start < end) {
		int pivot = do_partition(a, start, end);
		my_qsort(a, start, pivot - 1);
                my_qsort(a, pivot + 1, end);			
	}
}
