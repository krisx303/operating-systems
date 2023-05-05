#define SEM_BARBERS 1 // Semafor sygnalizujący zapotrzepowanie na fryzjera, jeśli wynosi więcej niż 0, to oznacza że jakiś klient czeka na fryzjera
#define SEM_CHAIRS 2  // Semafor sygnalizujący dostępność krzeseł w salonie
#define SHARED_MEM 3  // Semafor odpowiadający za dostęp do pamięci wspólnej
#define SEM_QUEUE 4   // Semafor odpowiadający za wrzucanie klientów do kolejki (czekając na fryzjera)
                      // Ustawiana jest na nim wartość krzeseł aby zaznaczyć że tyle klientów może być obsłużonych naraz, a reszta musi spać (czekać na fryzjera)

// Dane wejściowe

#define CHAIRS 3
#define WAITING 2
#define QUEUE_SIZE CHAIRS + WAITING
#define BARBERS 1
#define CLIENTS 2

#define HAIRCUTS 5
char HaircutTimes[] = {3, 4, 5, 6, 7};