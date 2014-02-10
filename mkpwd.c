main (argc, argv)
{
	printf ("Crypted: %s\n", crypt(getpass("Password: "), "mT"));
}
