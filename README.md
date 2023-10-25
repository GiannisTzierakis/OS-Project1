# OS-Project1

**Εντολές make/εκτέλεσης:**

Το πρόγραμμα γίνεται make μ’ένα απλό make. Η make clean διαγράφει όλα τα .ο αρχεία. Η εντολή για να τρέξει το πρόγραμμα είναι η : ./requester βαθμός_κατάτμισης αριθμός_παιδιών αριθμός_αιτημάτων stream αρχείου. Με αυτή τη σειρά αυστηρά! 
π.χ.:   ./requester 10 2 3 input.txt

**Main(parent.c):**

Διαβάζω από τη γραμμή εντολών βαθμό κατάτμισης, αριθμό παιδιών και request καθώς και το stream του αρχείου.
Ανοίγω το αρχείο.
Τα Segments τα κρατάω σε μια δομή όλα αφού τα δημιουργήσω στη main με τη συνάρτηση initialize_segments (utils.c) όπου κάνω τους κατάλληλους υπολογισμούς για να χωριστούν τα segments σωστά.

**Utils.h:**

Αποτελεί γενικό header file όπου γίνονται declare τα Structs: Memory και Segment. Επίσης έχουμε το declaration όλων των συναρτήσεων που χρειάζονται είτε για initialization είτε για κάτι άλλο.

Έχουμε 2 initialize, μια για τα segments και μια για το shared memory .

**Utils.c:**

Εδώ υλοποιούνται οι συναρτήσεις που γίνονται declare στην utils.h. 

**Γενικές Παραδοχές:**

→ Χρησιμοποιούνται σεμαφόροι ίσοι με τον αριθμό των segments.
→ Οι σεμαφόροι δεν είναι binary και μετράνε τον αριθμό των αιτήσεων ανά segment ανά πάσα στιγμή. 
→ Ότι έχει να κάνει με χρόνο δεν πρόλαβα να το υλοποιήσω. 
→ Όταν το παιδί ζητάει ένα segment ο γονέας το περνάει ολόκληρο στο shmem αντί να περνάει μονάχα τη γραμμή.
→ Απ’ότι κατάλαβα το παιδί ζητάει μόνο τη γραμμή και με πράξεις βρίσκουμε σε ποιο segment βρίσκεται για να το περάσει ο γονέας στη shmem.
→ Κάνω την παραδοχή ότι έχω max 24 παιδιά γιατί είναι απαραίτητο για τον πίνακα στο struct memory
