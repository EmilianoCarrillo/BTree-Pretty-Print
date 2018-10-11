# BTree-Pretty-Print
This is a BTree class made on c++ which lets you print it on a pretty graphical way.

DISCLAIMER: I'm not the owner of the entire class. I'm just the owner of the printTree and printPage functions.

## View:
~~~~
      `---.66.
          |   `---.75.87.94.
          |       |  |  |   `---.96.98.99.
          |       |  |   `---.89.93.
          |       |   `---.77.81.
          |        `---.69.73.
           `---.22.35.
               |  |   `---.41.55.58.62.
               |   `---.26.29.
                `---.15.19.
~~~~

## Use:
~~~~
      struct btreeNode {
          int val[MAX+1], count;
          btreeNode *link[MAX + 1];
      };
      btreeNode *root;
      insertion(val);   // val = any given two-character int. 
      // ...
      printTree(root);
~~~~
