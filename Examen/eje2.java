// fumadores1: 1 estanquero y n fumadores.
// A cada fumador le falta un solo ingrediente.

import monitor.*;
import java.util.Random;

class Estanco extends AbstractMonitor{
	private int estado1 = -1;	
	private int estado2 = -1;
	private int nfumadores;
	private Condition[] fumadores;
	private Condition estanquero1 = makeCondition();
	private Condition estanquero2 = makeCondition();

	public Estanco(int nfumadores){
		this.nfumadores = nfumadores;
		fumadores = new Condition[nfumadores];

		for(int i=0; i < nfumadores; i++)
			fumadores[i] = makeCondition();
	}

	public int getFumadores(){
		return nfumadores;
	}
	
	public void obtenerIngredientes (int ingrediente){
		enter();

		if(Math.random ()%2 == 0){
		
			if(estado1 != ingrediente) 
				fumadores[ingrediente].await();
		
			System.out.println("El fumador " + estado + " está fumando");
			estado1 = -1;
			estanquero.signal();

		else{
			if(estado2 != ingrediente) 
				fumadores[ingrediente].await();
		
			System.out.println("El fumador " + estado + " está fumando");
			estado2 = -1;
			estanquero.signal();
		}

		leave();
	} 

	public void ponerIngrediente(int ingrediente){
		enter();

		if(estado1 == -1){
			estado1 = ingrediente;
			System.out.println("El fumador " + estado + " puede fumar");
			fumadores[estado1].signal();
		}

		if(estado2 == -1){
			estado2 = ingrediente;
			System.out.println("El fumador " + estado + " puede fumar");
			fumadores[estado2].signal();
		}
		
		leave();
	} 

	public void esperarRecogidaIngrediente(){
		enter();

		if(estado1 != -1)
			estanquero1.await();
		if(estado2 != -1)
			estanquero2.await();
		
		leave();
	}	
}	
	
class Fumador implements Runnable{
	private int ingrediente;
	private Estanco estanco;
	public Thread thr;

	public Fumador(Estanco estanco, int ingrediente, String nombre){
		this.ingrediente = ingrediente;
		this.estanco = estanco;
		thr = new Thread(this,nombre);
	}

	public void run(){
		while (true){
			estanco.obtenerIngredientes(ingrediente);
			aux.dormir_max(2000);		
		}
	}
}

class Estanquero implements Runnable{
	private Estanco estanco;
	public Thread thr;

	public Estanquero(Estanco estanco, String nombre){
		this.estanco = estanco;
		thr = new Thread(this,nombre);
	}
		
	public void run(){
		int ingrediente;
			
		while (true){
			ingrediente = (int) (Math.random () * estanco.getFumadores());			
			estanco.ponerIngrediente(ingrediente);
			estanco.esperarRecogidaIngrediente();
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

class EjemploFumadores{
	public static void main(String[] args){
		
		int nfumadores;		
	
		if (args.length != 1){
			System.out.println("Parámetros por defecto: 1 estanquero y 3 fumadores");
         nfumadores = 3;
      }else
      	nfumadores = Integer.parseInt(args[0]);
    	
		Estanco estanco = new Estanco(nfumadores);

		Estanquero estanquero = new Estanquero(estanco,"Estanquero");

		Fumador[] fumadores = new Fumador[nfumadores];

		for(int i=0; i < nfumadores; i++)
			fumadores[i] = new Fumador(estanco,i,"Fumador "+i);

		estanquero.thr.start();

		for(int i=0; i < nfumadores; i++)
			fumadores[i].thr.start();
	}
}
