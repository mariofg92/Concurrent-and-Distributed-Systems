/* barberia1: 1 barbero n clientes */

import monitor.*;
import java.util.Random ;

class Barberia extends AbstractMonitor{
	private int clientesPelados = 0;
	private int clientesLlendose = 0;
	private boolean durmiendo = true;
	private Condition sala;
	private Condition barbero;

	public Barberia(){
		sala = makeCondition();
		barbero = makeCondition();
	}

	// invcado por los clientes para cortarse el pelo
	public void cortarPelo(){ 
		enter();

		if(durmiendo){
			System.out.println ("El cliente " + clientesPelados + " despierta al barbero");
			durmiendo = false;
			barbero.signal();
			clientesPelados++;
		}else{
			System.out.println("Entra un nuevo cliente mientras el barbero está ocupado");
			sala.await();
		}

		leave();
	}

	// invocado por el barbero para esperar (si procede) a un nuevo cliente y sentarlo para el corte
	public void siguienteCliente(){
		enter();

		if (sala.isEmpty()){
			System.out.println("El barbero duerme");
			durmiendo = true;
			barbero.await();
		}else{
			System.out.println("El barbero sienta al cliente " + clientesLlendose);
			sala.signal();		
		}

		leave();
	}

	// invocado por el barbero para indicar que ha terminado de cortar el pelo
	public void finCliente(){ 
		enter();

		System.out.println("El barbero ha terminado de pelar al cliente " + clientesLlendose);
		clientesLlendose++;

		leave();
	}
}

class Cliente implements Runnable {
	private Barberia barberia;
	public Thread thr;

	public Cliente(Barberia barberia, String nombre){
		this.barberia = barberia;
		thr = new Thread(this,nombre);
	}

	public void run(){
		while (true){
			barberia.cortarPelo(); // el cliente espera (si procede) y se corta el pelo
			aux.dormir_max(2000); // el cliente esta fuera de la barberia un tiempo
		}
	}
}

class Barbero implements Runnable{
	private Barberia barberia;	
	public Thread thr;

	public Barbero (Barberia barberia, String nombre){
		this.barberia = barberia;
		thr = new Thread(this,nombre);
	}

	public void run(){ 
		while(true){
			barberia.siguienteCliente();
			aux.dormir_max(2000); // el barbero esta cortando el pelo
			barberia.finCliente();
		}
	}
}

class aux{
	static Random genAlea = new Random();
	
	static void dormir_max(int milisecsMax){
		try{
			Thread.sleep(genAlea.nextInt(milisecsMax));
		}catch(InterruptedException e){ 
			System.err.println("sleep interumpido en ’aux.dormir_max()’");
		}
	}
}

class EjemploBarberia{
  	public static void main(String[] args){
		
		int nclientes;		
	
		if(args.length != 1){
			System.out.println("Parámetros por defecto: 1 barbero y 3 clientes");
         nclientes = 3;
      }else
      	nclientes = Integer.parseInt(args[0]);
    	
		Barberia barberia = new Barberia();

		Barbero barbero = new Barbero(barberia,"Barbero ");

		Cliente[] clientes = new Cliente[nclientes];

		for(int i=0; i < nclientes; i++)
			clientes[i] = new Cliente(barberia,"Cliente "+i);

		barbero.thr.start();

		for(int i=0; i < nclientes; i++)
			clientes[i].thr.start();
  	}
}
