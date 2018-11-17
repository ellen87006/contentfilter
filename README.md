# contentfilter
### 1. Implement
Filter tree with binary tree.

Time complixity :

  - Building filter : O(n^2) n = quantity of Profanities

  - Remove Profanities: O (n^2) n = size of character

  - Space complixity :O(2n) 

### 2. Benchmark
Test cases:

  - Get profanities form https://raw.communitydragon.org/7.24/plugins/rcp-fe-language-filter/global/

  - Addtional about 17K. 

  - Test performance with utils/testcase.txt  

  - Raspberry Pi 3 Model B+ with Linux raspberrypi 4.14.79-v7+.

|| B-Tree Filter | Trie | Brute filter |
| ------ | ------ |------ |------ |
| Pre Building Tree | 17970283 Tick |219860 Tick | 0 Tick |
| Sentence Average time |153 Tick | 38 Tick |22010 Tick |

