**Flight Management System**

This is a C++ command-line application developed to provide user-friendly flight management, ticket booking, secure payments, and passenger check-in services.

## Program Functions

The system is split into three primary functional sub-sections, backed by custom file-based data storage:

### 1. User Authentication (Login Modules)
* **Registration:** Allows new users to create accounts by entering credentials, including strict validation criteria for secure passwords. User details are appended to `user.txt`.
* **Login & Authentication:** Securely reads existing records from `user.txt` to parse and validate matching usernames and passwords to grant application access.

### 2. Flight Ticket Management (Booking Modules)
* **Perform Booking:** Prompts users to select fixed schedules, routes, time slots, and specify passenger volumes. 
* **Edit Booking:** Gives users the ability to modify specific trip details (like departure slots or travel dates) across group bookings.
* **Persistent Storage:** Records dynamically map to user-specific local tracking files labeled `XX_Booking.txt` (where `XX` is the account username).

### 3. Transactions & Pre-Flight Administration (Payment & Check-In Modules)
* **Perform Payment:** Calculates total itinerary amounts and processes transactions via credit/debit or bank transfer.
* **Passenger Check-In:** Restricts check-in capabilities until payment verification clears. Collects necessary passport and individual contact markers for travelers.
* **Log Auditing:** Synchronizes transactional state updates instantly into custom file paths labeled `XX_paymentCheckIn.txt`.

### Bonus Feature
* **Invoice Generation:** Generates receipt containing comprehensive trip details, including payment info, flight schedule, passenger details to localized text logs titled `XX_invoice.txt`.
