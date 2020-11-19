/* Versión con dos barberos */

import monitor.*;
import java.util.Random;

class Barberia extends AbstractMonitor{
	private Condition [] barbero = new Condition[2];
	private Condition sala;
	private boolean barbero0Ocupado = false;
	private boolean barbero1Ocupado = false;
	private int clientesPelados = 0;
	private int clientesLlendose = 0;
	
	public Barberia(){
		sala = makeCondition();
		barbero[0] = makeCondition();
		barbero[1] = makeCondition();
	}
	
	
	public void cortarPelo(){	
		enter();
			
		if((!barbero0Ocupado || !barbero1Ocupado) && sala.isEmpty()){
			if(!barbero0Ocupado){
				barbero[0].signal();
				System.out.println("El barbero 0 esta pelando al cliente " + clientesPelados);
				barbero0Ocupado = true;
			}else if(!barbero1Ocupado){
				barbero[1].signal();
				System.out.println("El barbero 1 esta pelando al cliente " + clientesPelados);
				barbero1Ocupado = true;
			}
			clientesPelados++;
		}else if(barbero0Ocupado && barbero1Ocupado){
			sala.await();
			System.out.println("Ha entrado un nuevo cliente mientras los barberos estan ocupados");
		}
		
		leave();
	}
	
	
	public void siguienteCliente(){
		enter();
		
		if((!barbero0Ocupado || !barbero1Ocupado) && !sala.isEmpty()){
			sala.signal();
			System.out.println("Puede pasar el siguiente");
		}else if(!barbero0Ocupado && sala.isEmpty()){
			barbero[0].await();
			System.out.println("El barbero 0 se va a dormir");
		}else if(!barbero1Ocupado && sala.isEmpty()){
			barbero[1].await();
			System.out.println("El barbero 1 se va a dormir");
		}

		leave();
	}
	
	public void finCliente(){
		System.out.println("El cliente " + clientesLlendose + " esta saliendo");

		if (clientesLlendose % 2 == 0) 
			barbero0Ocupado = false;
		else
			barbero1Ocupado = false;

		clientesLlendose++;
	}
}


class Cliente implements Runnable{
	private Barberia barberia;
	public Thread thr;

	public Cliente(Barberia barberia, String nombre){
		this.barberia = barberia;
		thr = new Thread(this,nombre);
	}
	
	public void run(){
		while (true){
			barberia.cortarPelo(); // el cliente espera (si procede) y se corta el pelo
			aux.dormir_max(2000);  // el cliente está fuera de la barbería un tiempo
		}
	}
}


class Barbero implements Runnable{
	private Barberia barberia;
	public Thread thr;

	public Barbero(Barberia barberia, String nombre){
		this.barberia = barberia;
		thr = new Thread(this,nombre);
	}	
	
	public void run(){
		while (true){
			barberia.siguienteCliente();
			aux.dormir_max(2000);  // el barbero está cortando el pelo
			barberia.finCliente();
		}
	}
}

class aux{
	static Random genAlea = new Random();
	static void dormir_max(int milisecsMax){
		try{
			Thread.sleep(genAlea.nextInt(milisecsMax));		
		}
		catch(InterruptedException e){
			System.err.println("Sleep interrumpido en 'aux.dormir_max()'");
		}
	}
}

class EjemploBarberia2{
	public static void main(String[] args){
   	Barberia barberia = new Barberia();

		Barbero barbero = new Barbero(barberia,"barbero");
		Cliente cliente0 = new Cliente(barberia,"cliente 0");
		Cliente cliente1 = new Cliente(barberia,"cliente 1");
		Cliente cliente2 = new Cliente(barberia,"cliente 2");


		barbero.thr.start();
		cliente0.thr.start();
		cliente1.thr.start();
		cliente2.thr.start();
	}
}
